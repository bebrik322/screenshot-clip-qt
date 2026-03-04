#include <QCoreApplication>
#include <QProcess>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    bool nosave = std::getenv("SCREENSHOT_NOSAVE") != nullptr;

    QString full_path;
    if (nosave) {
        full_path = QDir::tempPath() + QString("/screenshot-clip-%1.png")
                        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss"));
    } else {
        QString screenshot_dir;
        const char *env_dir = std::getenv("SCREENSHOT_DIR");
        if (env_dir) {
            screenshot_dir = QString::fromUtf8(env_dir);
        } else {
            const char *home = std::getenv("HOME");
            if (home) {
                screenshot_dir = QString("%1/Screenshots").arg(QString::fromUtf8(home));
            } else {
                std::cerr << "Neither SCREENSHOT_DIR nor HOME environment variables set" << std::endl;
                return 1;
            }
        }

        QDir dir(screenshot_dir);
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                std::cerr << "Failed to create screenshot directory" << std::endl;
                return 1;
            }
        }

        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
        full_path = QDir(screenshot_dir).filePath(QString("screenshot-%1.png").arg(timestamp));
    }

    QProcess spectacle;
    spectacle.start("spectacle", QStringList() << "-r" << "-b" << "-n" << "-o" << full_path);

    if (!spectacle.waitForFinished(-1)) {
        std::cerr << "spectacle process failed" << std::endl;
        return 1;
    }

    if (spectacle.exitCode() != 0) {
        std::cerr << "spectacle exited with non-zero status" << std::endl;
        return 1;
    }

    QFileInfo fileInfo(full_path);
    if (!fileInfo.exists())
        return 0;

    QFile png_file(full_path);
    if (!png_file.open(QIODevice::ReadOnly)) {
        std::cerr << "Failed to open screenshot for copying" << std::endl;
        return 1;
    }
    QByteArray png_data = png_file.readAll();
    png_file.close();

    QProcess wlCopy;
    wlCopy.start("wl-copy", QStringList() << "--type" << "image/png");
    wlCopy.write(png_data);
    wlCopy.closeWriteChannel();
    
    if (!wlCopy.waitForFinished(5000)) {
        std::cerr << "wl-copy failed" << std::endl;
        return 1;
    }

    if (nosave)
        QFile::remove(full_path);

    return 0;
}

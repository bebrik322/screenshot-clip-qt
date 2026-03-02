# screenshot-clip-qt

Small helper tool for KDE Plasma **Wayland** that takes a screenshot with Spectacle and puts it into the clipboard using Qt's native `QClipboard` API, so Klipper reliably tracks it in its history.

Many users on Plasma Wayland see a weird bug: Spectacle says the screenshot is copied, it even appears in the clipboard history, but:
- the entry looks empty / with a broken preview, or  
- pasting does nothing until you first select some other history item and then select the screenshot again.

`screenshot-clip-qt` works around this by saving the screenshot to disk as PNG, loading it via Qt and pushing a proper `image/png` into `QClipboard`.

## Usage

```bash
screenshot-clip
```

Default behavior:

- Takes a region screenshot using Spectacle.
- Saves it to ~/Screenshots/ as screenshot-YYYY-MM-DD_HH-mm-ss.png.
- Copies the image to the clipboard in a way that Klipper can see and paste it.

You can change the save directory with the SCREENSHOT_DIR environment variable:

```bash
SCREENSHOT_DIR="$HOME/Pictures/Screenshots" screenshot-clip
```

To take a screenshot without saving it to disk, use the `--no-save` flag:

```bash
screenshot-clip --no-save
```

The image goes straight to the clipboard and the temporary file is deleted.

Bind `screenshot-clip` to a global shortcut in System Settings → Shortcuts and use it instead of the default Spectacle shortcut. The only change to your normal workflow is that you will now need to press `Ctrl` + `S` to finalize the capture.

## Build

Requires Qt 6 (Core, Gui) and CMake.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build
```

## Install

```bash
cp build/screenshot-clip-qt ~/.local/bin/screenshot-clip
```

Make sure ~/.local/bin is in your PATH.

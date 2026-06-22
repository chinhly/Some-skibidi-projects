# Flappy Bird (Flappy Ghost)

A classic Flappy Bird clone built using Python and Pygame. Navigate the bird through the gap in the pipes without hitting them or falling off the screen!

---

## 🚀 How to Download and Run the Game

To play the game on Windows, follow these simple steps:

### 1. Download the Game Folder
* Download the game as a **ZIP** file by clicking the download button on the hosting page (e.g., GitHub, Google Drive, or your distribution link).
* Save the ZIP file to your preferred location (e.g., your **Downloads** or **Desktop** folder).

### 2. Extract (Unzip) the Folder
> [!IMPORTANT]
> **Do not run the game directly from inside the ZIP file!** You must extract it first so the game can access its images and sound files.
* **Right-click** on the downloaded `.zip` file.
* Select **Extract All...** from the context menu.
* Choose a destination folder and click **Extract**.

### 3. Run the Game
> [!WARNING]
> **Keep the files together!** The `flappyBird.exe` file **must** remain in the same folder as the `assets` directory. If you move `flappyBird.exe` by itself to another folder or your desktop, the game will crash when opened because it won't be able to find the sprites and sounds.

* Open the extracted folder.
* Double-click **`flappyBird.exe`** to start the game!

---

## 🎮 How to Play

* **Start Game / Jump**: Press the **`Spacebar`** key on your keyboard.
* **Navigate**: Tap **`Spacebar`** repeatedly to flap your wings and stay airborne.
* **Restart / Go Home**: After a game over, click the **`Left Mouse Button`** on the screen to return to the home screen.

---

## 🛠️ Troubleshooting

### Windows Protected Your PC (SmartScreen Warning)
Since the game is packaged as an independent executable (`.exe`), Windows Defender SmartScreen might show a warning when you open it for the first time:
1. Click on **More info** in the dialog box.
2. Click **Run anyway**.

### Game Crashes Immediately on Open
If the game closes instantly or fails to open:
1. Check that the **`assets`** folder is present in the same folder as `flappyBird.exe`.
2. Make sure you extracted the files from the ZIP folder instead of double-clicking inside the ZIP viewer.

---

## 💻 Running from Source Code (Optional)

If you are a developer and want to run or modify the Python source code directly:

1. Ensure you have Python installed.
2. Install Pygame via pip:
   ```bash
   pip install pygame
   ```
3. Run the game script:
   ```bash
   python flappyBird.py
   ```

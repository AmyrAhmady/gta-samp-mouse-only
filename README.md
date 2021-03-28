# Play GTA SAMP with your mouse and no keyboard!
This is basically a mod for GTA SAMP to play with mouse only, and it's a way to perform basic main actions like jumping, crouching, walking around, driving and etc.

(Yes I know ther eare some repetitive code or basically everything thrown into one file to handle all actions, I'll get it sorted when I add support for other SAMP clients)

## Showcase
https://youtu.be/0bnMW2-b6dg

## Notes

- This mod only works with ***SA:MP 0.3.7 R1*** (not even gtasa singleplayer).
- In order to use an on-screen keyboard, you need to run GTASA in windowed mode, there are multiple ASI plugins for that and one can be found here: 
https://ugbase.eu/threads/gta-sa-multiprocess-updated.4100/
- You need to enable Steer with Mouse in **Options**, **Controller Setup**, **Mouse Settings** to be able to do wheel steering while driving.
- You also have to bind jump action to **SPACE** key instead of **Left Shift** in order letting this mod simulate the key press for you.
- You need to disable cursor mode first to go to game's pause menu

## Installation

1. You need [SAMP 0.3.7 R1](https://github.com/KrustyKoyle/files.sa-mp.com-Archive/blob/master/sa-mp-0.3.7-install.exe) installed.
2. To use this mod you need an ASI loader, I recommend [Silent's ASI Loader](https://www.gtagarage.com/mods/show.php?id=21709) since I've been using it without any issue over past 10 years, all you have to do is copy contents to your GTA SA installation directory
3. Go to [Releases](https://github.com/AmyrAhmady/gta-samp-mouse-only) and download latest version ASI plugin and copy the `.asi` file into your GTA SA installation directory

## Usage

- **Double Middle button click**: To enable cursor; If you are running GTA SA in windowed mode and an on screen keyboard, you can do this to use your virtual keyboard, usefful for sending messages or performing commands (like you can click on T key on your OSK to open chat text box to write on it using your OSK) or other purposes if needed.
- **Middle button click**: Move forward, both **on foot** and **in vehicle** (simulates holding `W` key).
- **Forward button click**: Crouch, pressing it again after crouching would make your character stand up.
- **Back button click[ON FOOT]**: Jump (You HAVE to rebind your JUMP action key to SPACE key to make this work!).
- **Back button click[IN VEHICLE]**: Simulates pressing and holding `S` key, to perform a slow brake or driving reverse (You can even do a burst out (is that the word?) by enabling this and mvoe forward).
- **Double Right click**: To enter and exit vehicles.
- **Triple Right click**: to enter a vehicle as a pssenger if possible (if there are untaken/available seats).

## Last words
If there is anything, any issue that are unknown to me, or there are ideas and suggestions from you, I'll be glad to give them a thought and implement them, you are always welcome to contribute or ask your questions in https://github.com/AmyrAhmady/gta-samp-mouse-only/issues (or give me ideas how to handle them)

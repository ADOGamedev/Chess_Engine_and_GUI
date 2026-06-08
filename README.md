# **CHESS ENGINE & GUI**
#### GitHub repo: https://github.com/ADOGamedev/Chess-Engine-and-GUI

#### Description: This is an application made in Godot 4.6 and C++ through GDExtensions[^1]. On the left there is the board where you play. On the right there are multiple options:

### **Main menu**

- **Time mode**: select a time mode by clicking on the corresponding button.  
> [!NOTE]
> The syntax `3|2` means you have 3 minutes, and after each move, 2 seconds are added to your time.

- **Color**: the color you will play as. The black and white button randomizes it.

- **Play against AI**: play against the game engine selected in settings (the default is my own, ADOCE).

- **Play**: after you click this button, the game starts.

- **Flag buttons**: after the game has started, these are used to stop the game (resign).

- **Arrow buttons**: used to see which moves were played.  
> [!CAUTION]
> If you use these arrows to view a castling move, weird things may happen and it might even crash.  
> I will fix this eventually.

### **Settings Menu** 

Accessed by clicking the gear icon in the top-right corner.

- **UCI engine path**: the path to the executable of the engine used.  

- **FEN of current position**: the current position written in FEN[^3] notation.

- **Starting FEN**: the position (written in FEN[^3] notation) from which the game will start.

- **Volume**: controls the volume of subtle sound effects when a piece is moved.

## Screenshots

![A screenshot of the app](screenshots/screenshot1.png)
![A screenshot of the app](screenshots/screenshot2.png)
![A screenshot of the app](screenshots/screenshot3.png)
![A screenshot of the app](screenshots/screenshot4.png)
![A screenshot of the app](screenshots/screenshot5.png)

[^1]: https://docs.godotengine.org/en/4.4/tutorials/scripting/gdextension/gdextension_cpp_example.html  
[^2]: https://en.wikipedia.org/wiki/Universal_Chess_Interface  
[^3]: https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
# ConflictSolverEXEGameEngineCheckIn

**Group 4**

**Team formation:**  
We decided to get together based on us being in the same group for our GDW project. We agreed because it’s much easier and beneficial to use what we learned when working in this project to improve and help develop the game comfortably.

**The team:**  
Anson \- Designed Combat System, Character stat viewer plugin  
Matt \- SFX designer, Music Composer, Audio Manager  
John \- Made 3D VFX, Created key spawner, created normal maps  
Minh \- Enemy 3D modeller and animator, VFX artist, Performance tracker plugin  
Charleen \- 2D/3D artist, animator, technical artist, material generator plugin

**Gantt Chart:**  
![][image1]

**Interactive Media Scenario:**  
Rogue-like rpg game, where your game project gets corrupted by BitHub. You call on ConflictSolver.exe to Solve. That. Conflict\!\!\!  Entirely done in Unreal Engine 5\.  
This is worked on our GDW game, the purpose of this scenario is to provide a fun game for the user to play. The player is expected to engage in battles and beat the final boss: BitHub. The game is top down, 2.5 D, and the player can move around in the x and y axis. They can engage in battles when touching a trigger zone to fight enemies. 

We used singletons, factory, and command patterns along with a couple plugins.

**Audio Manager logic:**

* Singleton by design as it persists across different levels.  
* It handles all BGM with events to play, stop, pause and resume. Also playing another track will make the audio fade out and fade into the next one.

Diagram:

**![][image2]**  
**![][image3]**  
**Spawner Logic:**   
Taken straight from the Unreal slides on canvas, it uses a class reference of the blueprint instance of the key and summons it upon the custom event being called on

There is logic for spawning in items to be picked up on the prologue  
Useful for spawning in entities with only know class references and not specific details/types  
Especially good for (when we get to it) spawning different enemy encounters for combat or possibly upgrades  
Diagram:  
![][image4]  
![][image5]  
![][image6]

**Enemy Encounter Spawner:**  
Basically, combat will be triggered when the player is walking around the level and touches a trigger area. These areas are spawned using this factory.

The data table is used for enemy layout and each time the player enters the battle it will pick from the potential layouts from the table. All battle character is an actor so we could just cast to actor and spawn them

Diagram:  
![][image7]

**![][image8]**

**Command Pattern:**

**PLUGINS:**  
Plugins created: Performance Tracker, Stat viewer, Material Generator  
With the help of AI

Performance Tracker:

* The subsystem will automatically initializes when the game starts and uses FTickableGameObject to run a data collection loop on every rendered frame, and asks the computer's operating system how much RAM the game is using, and checks with the graphics system (RHI) to see how much VRAM is being used by game textures  
* UPerformanceTrackingBPLibrary acts as a bridge, converting all those backend C++ calculations into easy-to-use Blueprint nodes  
* By using GetCurrentFPS(), GetUsedMemoryMB(), GetUsedVRAMMB() to blueprint, the dev can see the performance stat in game. 




Stat Viewer:  
Before this plugin, it was very difficult to check character stats because we had to click on a lot of stuff just to get to one of the character stats. This plugin helps with showing all of the active characters stats on a single window.

* Custom widget using Unreal’s UI: Slate header and ToolMenus  
* Contains only the refresh button, which refreshes the current display and updates to the current data  
* Access the actor’s properties by finding and reading the attributes and accessing the right blueprints  
* The rest is just formatting the display for readability  
* The display also updates in real time using onTick()

Material Generator:

* Uses unreal’s headers  
  * slate, toolmenus, widget headers for ui to make custom widget  
  * Assettool headers for asset creation api  
  * Contentbrowsermodule for access to content browser (to select the parent material and to put new material in it)  
  * Miscellaneous header for message dialog (warnings)  
* Majority of the code is just to make the widget (rip)  
* Really only 1 method that does this:  
  * GenerateMaterial()  
* GenerateMaterial()  
  * Grab and access the content browser  
    * This is to select the assets in it, aka the parent material  
    * Also select the object that the material will apply to  
  * Create a folder for these materials  
  * Create a unique name for it with the appropriate suffix  
  * Modify the params from the material to fit whatever you need  
  * Then set the material to the object’s material component (all the slots)

Flowchart  
![][image9]

[Game Engine Check In document.pdf](https://github.com/user-attachments/files/34161204/Game.Engine.Check.In.document.pdf)

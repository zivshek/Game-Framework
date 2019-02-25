# 18W_FinalProject
Eternal Darkness, created with C++, OpenGL, [imgui](https://github.com/ocornut/imgui), [Bullet3](https://github.com/bulletphysics/bullet3), [IXAudio2](https://docs.microsoft.com/en-us/windows/desktop/api/xaudio2/nn-xaudio2-ixaudio2) 

**Techniques:** Postprocessing, skybox, multiple lighting, 3D mesh rendering, text rendering, fbx model importing, sound queue, resource management, scene management, event queue, debug draw, tweening, particle system, camera work, JSON file save/load

**Note:** In order to build the project on your machine correctly, you probably would have to retarget the solution to the SDK version that's installed on your machine.

*****************************
  Controls:

   - "Esc" to go back to last scene.
   
   - Menus:
     - Up/Down to navigate between menu items.
     - Space to select.
     - Left/Right to toggle.
   - GameScene:
     - WASD to move.
     - 'I' to open inventory.
     - 'O' to open in-game menu.
     - 'P' to open imGui window.
*****************************


### Game Programming Requirements (40%):

 7% - Game rules/Physics
  - Hunger system
  - Collect things (pick up, small blue cubes are apples, green cubes are mushrooms), Break stuff (trees are breakable), both inherited from Interactable, using raycast to detect them within a certain reach.
  - bullet physic engine two feature: Two Point Two Constraint, Ray Test
  - Joints (bridge)
  - Player character only moves to the direction of the camera.
  
 3% - Audio
  - Audio queues
  - Looping sounds
  - One-time sounds

 3% - Saved game
  - Saves survived time
  - Saves volume
  - Automatically saves while closing game.

 4% - Scene management
  - HUD will be drawn with game scene.
  - Total of 7 scenes working, dead scene not working~ will break for some reason, didn't put it in.
 
 3% - Tweening
  - Menu items

10% - Polish
  - FPS counter
  - Mouse control(turn camera around)
  - Inventory part 1 (logic)
  - Randomly generated world
  - Interactable things (nice hierarchy: GameObject->Interactable->Pickup/Breakable->Concrete Pickups/Breakables) using raytest
  
     
### Graphics Requirements (35%):

10% - Lighting
  - Multiple lighting, share functions between shaders, on and off functionality, below is a demonstration of spot light, directional light, and fog effect:
  ![lighting&fog](../assets/lighting.png)
  
 5% - Rendering to a texture
  - working, below is a demonstration of postprocessing:
  ![postprocessing1](../assets/postprocessing1.png)
  
 5% - Cubemaps
  - 2 cube maps for 2 levels, below is a demonstration of one of the skyboxes:
  ![skybox](../assets/skybox.png)
  
10% - Polish
  - Add fog effect.
  - Rendering to a texture for each scene, so postprocessing won't affect scenes other than game scenes
  - Text rendering
  - Particles
  - debug draw set up, but not done
  - Inventory part 2 (displaying apple and mushroom upon picking up)


### Testing: 5%
  - ImGui - testing the poisonous condition, below is a demonstration of using imgui:
  ![imgui](../assets/imgui.png)
  


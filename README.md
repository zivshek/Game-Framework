# 18W_FinalProject
Eternal Darkness, created with C++, OpenGL, imgui, Bullet3, IXAudio2 

Techniques: Postprocessing, skybox, multiple lighting, 3D mesh rendering, text rendering, fbx model importing, sound queue, resource management, scene management, event queue, debug draw, tweening, particle system, camera work, JSON file save/load

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
  - Hunger system (for some reason, my textrenderer will interfere with my game scene, so I didn't draw the HUD texts)
  - Collect things (pick up, small blue cubes are apples, green cubes are mushrooms), Break stuff (trees are breakable), both inherited from Interactable, using raycast to detect them within a certain reach.
  - bullet physic engine two feature: Two Point Two Constraint, Ray Test
  - Joints (bridge)
  - Player character only moves to the direction of the camera.

  
 3% - Audio
  - Audio queues
  - Looping sounds
  - One-time sounds

 3% - Saved game
  - Save survived time
  - Save volume
  - Automatically save while closing game.

 4% - Scene management
  - HUD will be drawn with game scene.
  - Total of 7 scenes working, dead scene not working~ will break for some reason, so we didn't put it in.
 
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
  - Multiple lighting, share functions between shaders, on and off functionality 
  
 5% - Rendering to a texture
  - working
  
 5% - Cubemaps
  - 2 cube maps for 2 levels
  
10% - Polish
  - Add fog effect.
  - Rendering to a texture for each scene, so postprocessing won't affect scenes other than game scenes
  - Text rendering
  - Particles
  - debug draw set up, but not done
  - Inventory part 2 (displaying apple and mushroom upon picking up)


### Testing: 5%
  - ImGui - testing the poisonous condition


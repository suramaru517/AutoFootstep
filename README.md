# AutoFootstep

## Setup

### 1. Open Modifiers Window

Open an animation sequence or skeleton to which you want to add footstep animation notifies or sync markers, and then open the **Animation Data Modifiers** window.

![AutoFootstep_Setup_01](https://github.com/suramaru517/AutoFootstep/assets/50897830/47e5e194-0679-4e49-995a-3b12876956f9)

### 2. Add Modifier

Add **AutoFootstepAnimationModifier** and set its properties.

<details>
 <summary>Description of each property</summary>

 ####
 * **Path Filters**  
   If the array is not empty, apply the modifier only to animations whose paths contain any of the strings in the array. This is useful when applying the modifier to a skeleton.
 * **Foot Bone Names**  
   Names of the foot bones to detect footsteps.
 * Notify
   * **Add Notify**  
     If true, add animation notifies to the footstep frames.
   * **Notify Class**  
     Animation notify class to add. The default is AutoFootstepAnimNotify.
   * **Notify Params**  
     Parameters to pass to the animation notifies. Valid only if Notify Class is AutoFootstepAnimNotify.
   * **Notify Track Name**  
     Name of the notify track to which animation notifies will be added.
 * Sync Marker
   * **Add Sync Marker**  
     If true, add sync markers to the footstep frames.
   * **Marker Names by Foot Bone**  
     Names of the sync markers for each foot bone.
   * **Marker Track Name**  
     Name of the notify track to which synch markers will be added.
 * Advanced
   * **Frame Rate Override**  
     The frame rate to use for detecting footstep frames. If false, the animation frame rate is used. Higher values will give more accurate results.
   * **Ground Height Range**  
     The higher the value, the coarser the footstep detection. If unnecessary footsteps are detected, change the value.
   * **Grounded Threshold**  
     The higher the value, the coarser the footstep detection. If the footstep frames are shifted, change the value.
   * **Add Curves Of Location Z**  
     If true, add curves of the foot bone location Z used to detect footsteps.

</details>

<details>
 <summary>Use AutoFootstepAnimNotify to play effects based on surface type</summary>

 1. Define surface types in project settings.

    ![AutoFootstep_Setup_02_01](https://github.com/suramaru517/AutoFootstep/assets/50897830/467e3782-7ad3-4f28-95f5-81db1b708e04)

 2. Create a **Data Asset**.

    ![AutoFootstep_Setup_02_02](https://github.com/suramaru517/AutoFootstep/assets/50897830/e2653b4a-c055-4f84-926d-d48bed6c861a)

 3. Select **AutoFootstepEffectContext**.

    ![AutoFootstep_Setup_02_03](https://github.com/suramaru517/AutoFootstep/assets/50897830/08c41b4c-fc24-4b4b-a3e6-c755e7fd42ff)

 4. Open the data asset you created and define effects for each surface type. Pressing the "**Add All Surface Type Elements**" button will automatically add elements with all surface types as keys.

    ![AutoFootstep_Setup_02_04](https://github.com/suramaru517/AutoFootstep/assets/50897830/cb93c67e-8e63-46e4-af57-958ea0347ff4)

 5. Create as many **Physical Materials** as you define surface types.

    ![AutoFootstep_Setup_02_05](https://github.com/suramaru517/AutoFootstep/assets/50897830/17118405-3101-4c9a-abe4-568d52ef63d5)

 6. Open the physical materials you created and set a different **Surface Type** for each.

    ![AutoFootstep_Setup_02_06](https://github.com/suramaru517/AutoFootstep/assets/50897830/493638f7-b533-4174-8766-aa6cb78535f7)

 7. Set the physical material to a mesh or material, such as a floor, where you want to change the footstep effect. 

    ![AutoFootstep_Setup_02_07](https://github.com/suramaru517/AutoFootstep/assets/50897830/f37ab4cd-6fc3-4071-89e4-65e3ab38a1eb)

 8. Set AutoFootstepAnimNotify in **Notify Class** and the data asset you created in **Effect Context** of Notify Params.

</details>

![AutoFootstep_Setup_02](https://github.com/suramaru517/AutoFootstep/assets/50897830/6a37f275-f141-4f35-abb5-0892b05bce12)

### 3. Apply Modifier

Apply **AutoFootstepAnimationModifier** to automatically add animation notifies or sync markers to footstep frames.

![AutoFootstep_Setup_03](https://github.com/suramaru517/AutoFootstep/assets/50897830/cb7b7de2-e806-4164-a362-bba693ac2d07)

## Tips

### Customize with Your Animation Notify

Create a class that inherits from AutoFootstepAnimationModifier and override "**On Notify Added**" to control the parameters passed to animation notifies.

![AutoFootstep_Tips_01](https://github.com/suramaru517/AutoFootstep/assets/50897830/d891fc2a-99c2-412b-bc3f-ff01fca9d181)

### Manually Play Footstep Effects

Use "**Play Effect by Surface Type**" of AutoFootstepEffectContext.

![AutoFootstep_Tips_02](https://github.com/suramaru517/AutoFootstep/assets/50897830/abf36307-ae3a-45bf-85af-1a3e8df52ed5)

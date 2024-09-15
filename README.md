# AutoFootstep

## How to Use

### 1. Open Modifiers Window

Open an animation sequence or skeleton to add footstep notifies, and open the **Animation Data Modifiers** window.

![AutoFootstep_Setup_01](https://github.com/suramaru517/AutoFootstep/assets/50897830/47e5e194-0679-4e49-995a-3b12876956f9)

### 2. Add Modifier

Add `AutoFootstepAnimationModifier` in the window and set its properties.

<details>
 <summary>Settings to play Niagara and sound effect by surface type (Click to open)</summary>

 1. Define surface types in the project settings.

    ![AutoFootstep_Setup_02_01](https://github.com/suramaru517/AutoFootstep/assets/50897830/467e3782-7ad3-4f28-95f5-81db1b708e04)

 2. Create a data asset of `AutoFootstepEffectContext`.

    ![AutoFootstep_Setup_02_02](https://github.com/suramaru517/AutoFootstep/assets/50897830/e2653b4a-c055-4f84-926d-d48bed6c861a)

    ![AutoFootstep_Setup_02_03](https://github.com/suramaru517/AutoFootstep/assets/50897830/08c41b4c-fc24-4b4b-a3e6-c755e7fd42ff)

 3. Open the data asset and press the **Add All Surface Type Elements** button, then set `Niagara` and `Sound` for each surface type.

    ![AutoFootstep_Setup_02_04](https://github.com/suramaru517/AutoFootstep/assets/50897830/cb93c67e-8e63-46e4-af57-958ea0347ff4)

 4. Create physical materials for each surface type.

    ![AutoFootstep_Setup_02_05](https://github.com/suramaru517/AutoFootstep/assets/50897830/17118405-3101-4c9a-abe4-568d52ef63d5)

    ![AutoFootstep_Setup_02_06](https://github.com/suramaru517/AutoFootstep/assets/50897830/493638f7-b533-4174-8766-aa6cb78535f7)

 5. Set the physical material to a mesh or material, such as a floor, where you want to change the footstep effect. 

    ![AutoFootstep_Setup_02_07](https://github.com/suramaru517/AutoFootstep/assets/50897830/f37ab4cd-6fc3-4071-89e4-65e3ab38a1eb)

 6. Go back to the modifiers window, set `AutoFootstepAnimNotify` to `Notify Class` and the data asset to `Effect Context` in `Notify Params`.

</details>

![AutoFootstep_Setup_02_08](https://github.com/user-attachments/assets/e406bc84-6443-4081-8fc6-6fc9a8715fdf)

### 3. Apply Modifier

Apply `AutoFootstepAnimationModifier` in the window, then footstep notifies will be added automatically.

![AutoFootstep_Setup_03](https://github.com/suramaru517/AutoFootstep/assets/50897830/cb7b7de2-e806-4164-a362-bba693ac2d07)

## FAQ

### How can I pass values from the modifier to my AnimNotify class?

Create a child class of `AutoFootstepAnimationModifier` and override `On Notify Added`, then apply it to the animations.

![AutoFootstep_FAQ_01](https://github.com/user-attachments/assets/35462208-4eaa-46b5-b9b3-b353d8158022)

### How can I reuse the trace results?

Create a child class of `AutoFootstepAnimNotify` and override `On Trace Hit`, then add it to the animations.

![AutoFootstep_FAQ_02](https://github.com/user-attachments/assets/55056a6e-bb92-43f5-b914-52bfeca25900)

### I want to play the footstep effect manually

You can use `Play Effect by Surface Type` of `AutoFootstepEffectContext`.

![AutoFootstep_FAQ_03](https://github.com/user-attachments/assets/8049ba72-006c-444f-9ba0-d6cef00e857a)

### I want to apply the modifier to all walk/run animations

If you have a naming convention in the folders or animations, you can use `Path Filters` in the modifier to filter the animations to apply it.

### I am not sure if the notifies are working properly

You can set `Draw Debug Line` in the modifier to true to see if it works.

### Extra notifies are added

Please try to increase the value of `Ground Height Range` in the modifier.

### Notify frames are shifted

Please try to increase the value of `Grounded Threshold` in the modifier.

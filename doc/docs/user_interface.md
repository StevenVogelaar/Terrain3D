User Interface
=================


**Table of Contents**
* [Main Toolbar](#main-toolbar)
* [Keyboard Shortcuts](#keyboard-shortcuts)
* [Tool Settings](#tool-settings)
* [Asset Dock](#asset-dock)


## Main Toolbar

After properly installing and enabling the plugin, add a Terrain3D node to your Scene and select it. This will enable the editing tools.

```{image} images/ui_tools.png
:target: ../_images/ui_tools.png
```

The tools provide options for sculpting, texturing, and other features. Each button has a tooltip that describes what it does.

First, select the Region Tool (first one: square with a cross), and click the ground. This allocates space for you to sculpt and paint.

---

## Keyboard Shortcuts

The following mouse and keyboard shortcuts are available.

### General Keys
* <kbd>LMB</kbd> - Click the terrain to positively apply the current tool.
* <kbd>Ctrl + LMB</kbd> - **Inverse** the tool. Removes regions, color, wetness, autoshader, holes, navigation, foliage. Height picks first+. Use <kbd>Cmd</kbd> on **macOS**.
* <kbd>Shift + LMB</kbd> - Temporarily change to the **Smooth** sculpting tool.
* <kbd>Ctrl + Z</kbd> - **Undo**. You can view the entries in the Godot `History` panel.
* <kbd>Ctrl + Shift + Z</kbd> - **Redo**.
* <kbd>Ctrl + S</kbd> - **Save** the scene and all data.

+ To clarify **Ctrl + Height**, this picks the height at the mouse cursor first, then flattens the landscape at that height.

### Slope Filter

The slope filter on the bottom settings bar limits operations based on terrain slope. Don't confuse this with the slope sculpting tool on the left toolbar.

These operations support filtering by slope: **Paint**, **Spray**, **Color**, **Wetness**, **Instancer**.

* <kbd>LMB</kbd> - Add within the defined slope.
* <kbd>Ctrl + LMB</kbd> - Remove within the defined slope.
* <kbd>Alt + LMB</kbd> - Add with the slope inversed.
* <kbd>Ctrl + Alt + LMB</kbd> - Remove with the slope inversed.

### Sculpting Specific
* <kbd>Alt + LMB</kbd> - **Lift floors**. This lifts up lower portions of the terrain without affecting higher terrain. Use it along the bottom of cliff faces. See [videos demonstrating before and after](https://github.com/TokisanGames/Terrain3D/pull/409). 
* <kbd>Ctrl + Alt + LMB</kbd> - **Flatten peaks**. The inverse of the above. This reduces peaks and ridges without affecting lower terrain around it.

### Instancer Specific
* <kbd>LMB</kbd> - Add the selected mesh instance to the terrain.
* <kbd>Ctrl + LMB</kbd> - Remove instances of the selected type.
* <kbd>Ctrl + Shift + LMB</kbd> - Remove instances of **any** type.

### Special Cases

**macOS Users:** Use <kbd>Cmd</kbd> instead of <kbd>Ctrl</kbd>.

**Maya Users:** The <kbd>Alt</kbd> key can be changed to Space, Meta (Windows key), or Capslock in `Editor Settings / Terrain3D / Config / Alt Key Bind` so it does not conflict with Maya input settings `Editor Settings / 3D / Navigation / Navigation Scheme`.

**Touchscreen Users:** will see an `Invert` checkbox on the settings bar which acts like <kbd>Ctrl</kbd> to inverse operations.


---

## Tool Settings

Depending on which tool is selected on the toolbar, various tool settings will appear on the bottom bar.

```{image} images/ui_tool_settings.jpg
:target: ../_images/ui_tool_settings.jpg
```

Many tool settings offer a slider with a fixed range, and an input box where you can manually enter a much larger setting.

Click the label of any setting to reset the value to its default, or checkboxes to toggle.

The settings are saved across sessions in `Editor Settings / Terrain3D / Tool Settings`. 

Some tools like `Paint`, `Spray`, and `Color` have options to disable some features. e.g. Disabling `Texture` on `Paint` means it will only apply scale or angle. Enabling `Texture` on `Color` will filter color painting to the selected texture.

On the right, the three dots button is the advanced menu. One noteworthy setting is `Jitter`, which is what causes the brush to spin while painting. Reduce it to zero if you don't want this.

Brushes can be edited in the `addons/terrain_3d/brushes` directory, using your OS folder explorer. The folder is hidden to Godot. The files are 100x100 alpha masks saved as EXR. Larger sizes should work fine, but will be slow if too big.

Most other options are self explanatory. See [Foliage Instancing](instancer.md) for specific details on its settings.

---

## Asset Dock


The asset dock houses the list of textures and meshes available for use in Terrain3D. The contents are stored in the [Terrain3DAssets](../api/class_terrain3dassets.rst) resource shown in the inspector when the Terrain3D node is selected.

Click `Textures` or `Meshes` to switch between the asset types.

Mesh thumbnail generation is currently unreliable. Hover over a mesh or click `Meshes` to regenerate them.

```{image} images/ui_asset_dock_bottom.png
:target: ../_images/ui_asset_dock_bottom.png
```

### Dock Position

The dropdown option allows you to select the dock position. Shown above, it is docked to the bottom panel. Below, it is docked to the right sidebar, in the [bottom right position](https://docs.godotengine.org/en/stable/classes/class_editorplugin.html#class-editorplugin-constant-dock-slot-left-ul).

```{image} images/ui_asset_dock_sidebar.png
:target: ../_images/ui_asset_dock_sidebar.png
```

The icon with white and grey boxes in the top right can be used to pop out the dock into its own window. While in this state, there is a pin button that allows you to enable or disable always-on-top.

Next the slider will resize the thumbnails.

Finally, when the dock is in the sidebar, there are three vertical, grey dots, shown in the image above, in the top right. This also allows you to change the sidebar position, however setting it here won't save. Ignore this and use our dropdown instead.


### Setting Up Assets

#### Adding Assets
You can add resources by dragging a texture onto the `Add Texture` icon, a mesh (a packed scene: tscn, scn, fbx, glb) onto the `Add Mesh` icon, or by clicking either `Add` button and setting them up. 

Each asset resource type has their own settings described in the API docs for [Terrain3DTextureAsset](../api/class_terrain3dtextureasset.rst) and [Terrain3DMeshAsset](../api/class_terrain3dmeshasset.rst).

You can read more about mesh setup on the [Foliage Instancer page](instancer.md#how-to-use-the-instancer).

#### Operations

<kbd>LMB</kbd> - Select the asset to paint with.

<kbd>RMB</kbd> - Edit the asset in the inspector. You can also click the pencil on the thumbnail.

<kbd>MMB</kbd> - Clear the asset. You can also click the X on the thumbnail. If this asset is at the end of the list, this will also remove it. You can clear and reuse this asset, or change its ID to move it to the end for removal. When using the instancer, this will remove all instances painted on the ground. It will ask for confirmation first.




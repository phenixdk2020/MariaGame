# MariaGame – Backlog

## P0 – Playable wardrobe prototype

- [x] Unreal C++ project bootstrap
- [x] Character customization design
- [x] Image-to-Clothing design
- [x] Virtual wardrobe design
- [x] Clothing item data model
- [x] Wardrobe inventory/equipment component
- [x] Physical hanger actor
- [x] Physical wardrobe actor with hanger rail
- [x] Modular Maria character slots
- [x] Runtime hair-color hook
- [x] Add placeholder body/character mesh
- [x] Add placeholder wardrobe mesh
- [x] Add placeholder hanger mesh
- [x] Add 5 placeholder clothing items
- [x] Player interaction trace
- [x] Highlight selected hanger/clothing
- [x] Pick clothing from hanger
- [x] Equip selected clothing on Maria
- [x] Return replaced clothing to hanger
- [x] Open/close wardrobe doors
- [x] Dressing-room preview mode
- [x] 360-degree character preview
- [x] Save/load outfit
- [ ] Interaction prompt UI
- [ ] Clothing item name UI
- [ ] Clothing category UI
- [ ] Crosshair / focus reticle
- [ ] Smooth wardrobe door animation
- [ ] Smooth hanger pickup animation
- [ ] Smooth equip transition
- [ ] Add interaction distance feedback
- [ ] Prevent interaction through closed wardrobe doors
- [ ] Add sound hooks for open/close/select/equip
- [ ] Add basic error/status messages
- [ ] Replace primitive wardrobe with production asset
- [ ] Replace primitive clothing with rigged test garments
- [ ] Replace cube hanger with production hanger
- [ ] Add test shoes to wardrobe
- [ ] Add test trousers to wardrobe
- [ ] Add test jacket to wardrobe
- [ ] Add test dress to wardrobe
- [ ] Add test accessories

## P1 – Core game framework

- [ ] Main menu
- [ ] New profile
- [ ] Load profile
- [ ] Settings menu
- [ ] Exit game
- [ ] Profile save slot naming
- [ ] Autosave
- [ ] Manual save
- [ ] Multiple user profiles
- [ ] Versioned save-game format
- [ ] Save migration for future versions
- [ ] Game instance / persistent session state
- [ ] Loading screen
- [ ] Global notification system
- [ ] Input rebinding
- [ ] Keyboard/mouse settings
- [ ] Controller support
- [ ] Basic accessibility options
- [ ] Localization-ready UI
- [ ] Danish UI text
- [ ] English UI text

## P2 – Avatar creator

### Photo capture/import
- [ ] Reference-photo import screen
- [ ] Front-photo slot
- [ ] 45° left-photo slot
- [ ] 45° right-photo slot
- [ ] Left profile-photo slot
- [ ] Right profile-photo slot
- [ ] Full-body front-photo slot
- [ ] Full-body side-photo slot
- [ ] Full-body rear-photo slot
- [ ] Drag/drop image support
- [ ] File picker support
- [ ] Image format validation
- [ ] Image resolution validation
- [ ] EXIF orientation handling
- [ ] Crop tool
- [ ] Rotate tool
- [ ] Face centering guide
- [ ] Pose alignment guide
- [ ] Lighting quality warning
- [ ] Blur detection
- [ ] Occlusion warning
- [ ] Hair-over-face warning
- [ ] Photo set completeness indicator
- [ ] Privacy notice / local handling description
- [ ] Delete imported source photos from profile

### Face/head system
- [ ] Face/head fitting pipeline
- [ ] Neutral base head mesh
- [ ] Facial landmark mapping
- [ ] Head width adjustment
- [ ] Face length adjustment
- [ ] Jaw width adjustment
- [ ] Chin shape adjustment
- [ ] Nose width/length adjustment
- [ ] Eye spacing adjustment
- [ ] Eye size adjustment
- [ ] Eyebrow shape adjustment
- [ ] Mouth width/fullness adjustment
- [ ] Ear size/position adjustment
- [ ] Skin tone extraction
- [ ] Skin material presets
- [ ] Freckles / moles layer
- [ ] Basic face texture projection
- [ ] Multi-view texture blending
- [ ] Face symmetry correction option
- [ ] Manual fine-tuning sliders
- [ ] Before/after photo comparison mode
- [ ] Face save/load

### Body system
- [ ] Body-reference capture guide
- [ ] Female neutral base body
- [ ] Height parameter
- [ ] Shoulder width parameter
- [ ] Chest parameter
- [ ] Waist parameter
- [ ] Hip parameter
- [ ] Arm length parameter
- [ ] Leg length parameter
- [ ] Torso length parameter
- [ ] Body depth parameter
- [ ] Hand size parameter
- [ ] Foot size parameter
- [ ] Body proportion morph targets
- [ ] Manual body tuning sliders
- [ ] Body measurement display
- [ ] Body measurement unit cm/inch
- [ ] Body preset save/load
- [ ] Prevent extreme/invalid morph combinations

### Hair system
- [ ] Hair selection browser
- [ ] Short hair style
- [ ] Medium hair style
- [ ] Long hair style
- [ ] Ponytail style
- [ ] Curly style
- [ ] Bob style
- [ ] Bun style
- [ ] Hair color presets
- [ ] Blonde preset
- [ ] Dark blonde preset
- [ ] Light brown preset
- [ ] Brown preset
- [ ] Dark brown preset
- [ ] Black preset
- [ ] Red preset
- [ ] Grey preset
- [ ] Free RGB/HSV hair color adjustment
- [ ] Root color
- [ ] Highlight color
- [ ] Ombre color
- [ ] Hair gloss
- [ ] Hair roughness
- [ ] Hair strand detail
- [ ] Eyebrow color link to hair
- [ ] Hair physics
- [ ] Hair collision with shoulders
- [ ] Hair collision with jackets
- [ ] Hat/headwear hair compatibility
- [ ] Save hair setup

### Avatar persistence
- [ ] Save avatar
- [ ] Load avatar
- [ ] Duplicate avatar
- [ ] Reset avatar to base
- [ ] Avatar thumbnail
- [ ] Avatar versioning
- [ ] Avatar data export
- [ ] Avatar data import

## P3 – Personal clothing import

### Import UI
- [ ] Clothing import screen
- [ ] Drag/drop clothing image
- [ ] File picker
- [ ] Camera/photo import workflow
- [ ] Front image slot
- [ ] Rear image slot
- [ ] Left side image slot
- [ ] Right side image slot
- [ ] Detail image slots
- [ ] Multi-image grouping per clothing item
- [ ] Crop clothing image
- [ ] Rotate clothing image
- [ ] Remove background helper
- [ ] Image quality indicator
- [ ] Missing-view warnings
- [ ] Clothing item name field
- [ ] Brand field
- [ ] Notes field
- [ ] Color tags
- [ ] Season tags
- [ ] Occasion tags

### Clothing analysis
- [ ] Clothing category detection
- [ ] T-shirt detection
- [ ] Shirt detection
- [ ] Blouse detection
- [ ] Sweater detection
- [ ] Hoodie detection
- [ ] Jacket detection
- [ ] Dress detection
- [ ] Skirt detection
- [ ] Jeans detection
- [ ] Trousers detection
- [ ] Shorts detection
- [ ] Shoe category detection
- [ ] Bag/accessory detection
- [ ] Sleeve length detection
- [ ] Neckline detection
- [ ] Fit estimation
- [ ] Garment length estimation
- [ ] Dominant color extraction
- [ ] Multi-color palette extraction
- [ ] Pattern detection
- [ ] Stripe detection
- [ ] Check/plaid detection
- [ ] Floral pattern detection
- [ ] Logo/print region detection
- [ ] Fabric appearance estimation
- [ ] Denim estimation
- [ ] Knit estimation
- [ ] Leather appearance estimation
- [ ] Glossy fabric estimation

### Clothing templates
- [ ] Template matching
- [ ] TShirt_Fitted template
- [ ] TShirt_Loose template
- [ ] Shirt_LongSleeve template
- [ ] Shirt_ShortSleeve template
- [ ] Blouse template
- [ ] Sweater template
- [ ] Hoodie template
- [ ] Jacket template
- [ ] Dress_Short template
- [ ] Dress_Long template
- [ ] Skirt template
- [ ] Jeans template
- [ ] Trousers template
- [ ] Shorts template
- [ ] Shoe low-top template
- [ ] Boot template
- [ ] Clothing template metadata
- [ ] Template gender/body compatibility
- [ ] Template versioning

### Texture/material creation
- [ ] Base color extraction
- [ ] Front texture projection
- [ ] Rear texture projection
- [ ] Side texture projection
- [ ] Multi-view texture blending
- [ ] Seam cleanup
- [ ] Logo placement
- [ ] Pattern scale adjustment
- [ ] Pattern rotation adjustment
- [ ] Roughness estimation
- [ ] Normal/detail map approximation
- [ ] Fabric preset selection
- [ ] User color correction
- [ ] User brightness adjustment
- [ ] User saturation adjustment
- [ ] Material preview sphere
- [ ] Unreal Material Instance generation

### Clothing fit
- [ ] Fit clothing to body proportions
- [ ] Chest morph matching
- [ ] Waist morph matching
- [ ] Hip morph matching
- [ ] Shoulder morph matching
- [ ] Arm length morph matching
- [ ] Leg length morph matching
- [ ] Garment length scaling
- [ ] Loose/tight fit slider
- [ ] Clothing offset
- [ ] Sleeve length fine tuning
- [ ] Hem length fine tuning
- [ ] Automatic body masking
- [ ] Manual body masking override
- [ ] Collision profile per garment
- [ ] Cloth simulation support
- [ ] Cloth self-collision
- [ ] Cloth/body collision
- [ ] Cloth/clothing collision
- [ ] Clipping detection
- [ ] Automatic clipping repair attempt
- [ ] Manual clipping adjustment tools

### Imported clothing persistence
- [ ] Clothing preview
- [ ] 360° clothing preview
- [ ] Save imported clothing to wardrobe
- [ ] Create physical hanger automatically
- [ ] Choose hanger type
- [ ] Choose shelf instead of hanger
- [ ] Thumbnail generation
- [ ] Clothing metadata save
- [ ] Edit imported clothing later
- [ ] Reprocess clothing item
- [ ] Delete clothing item
- [ ] Duplicate clothing item
- [ ] Clothing export/import package

## P4 – Virtual wardrobe

### Wardrobe layout
- [ ] Production wardrobe asset
- [ ] Left wardrobe section
- [ ] Right wardrobe section
- [ ] Upper shelf
- [ ] Lower shelf
- [ ] Shoe shelves
- [ ] Drawers
- [ ] Accessory tray
- [ ] Hanging rail
- [ ] Multiple hanging rails
- [ ] Wardrobe interior lighting
- [ ] Wardrobe door handles
- [ ] Mirror door option
- [ ] Open wardrobe option
- [ ] Closed wardrobe option
- [ ] Wardrobe material/color choices
- [ ] Wardrobe size variants

### Hangers
- [ ] Production hanger model
- [ ] Standard shirt hanger
- [ ] Wide jacket hanger
- [ ] Clip hanger for skirts
- [ ] Clip hanger for trousers
- [ ] Dress hanger
- [ ] Hanger spacing system
- [ ] Automatic hanger redistribution
- [ ] Hanger collision
- [ ] Hanger sway animation
- [ ] Empty hanger state
- [ ] Hanger label/tag
- [ ] Hanger selection animation

### Shelves and drawers
- [ ] Shoes on shelves
- [ ] Folded clothes on shelves
- [ ] Jeans folded on shelves
- [ ] Sweaters folded on shelves
- [ ] Bags on shelves
- [ ] Accessories in drawers
- [ ] Drawer open/close animation
- [ ] Shelf item selection
- [ ] Automatic shelf spacing
- [ ] Item stacking rules
- [ ] Drawer capacity rules

### Wardrobe organization
- [ ] Sort by clothing category
- [ ] Sort by color
- [ ] Sort by season
- [ ] Sort by brand
- [ ] Sort by recent use
- [ ] Sort by favorites
- [ ] Filter tops
- [ ] Filter bottoms
- [ ] Filter dresses
- [ ] Filter jackets
- [ ] Filter shoes
- [ ] Filter accessories
- [ ] Search wardrobe
- [ ] Favorite clothing
- [ ] Hide/archive clothing
- [ ] Custom wardrobe sections
- [ ] Drag item between wardrobe sections

### Wardrobe interaction
- [ ] Smooth wardrobe door animation
- [ ] Hand reach animation
- [ ] Take hanger from rail
- [ ] Return hanger to rail
- [ ] Pull item from shelf
- [ ] Return item to shelf
- [ ] Open drawer
- [ ] Close drawer
- [ ] Inspect clothing item
- [ ] Rotate inspected item
- [ ] Clothing detail panel
- [ ] Try-on button
- [ ] Wear button
- [ ] Remove button
- [ ] Put-back button
- [ ] Favorite button

## P5 – Dressing room / mirror

- [ ] Dedicated dressing-room mode
- [ ] 360° orbit camera
- [ ] Zoom camera
- [ ] Camera height adjustment
- [ ] Front view button
- [ ] Rear view button
- [ ] Left view button
- [ ] Right view button
- [ ] Full-body framing
- [ ] Face framing
- [ ] Shoes framing
- [ ] Mirror
- [ ] Real-time mirror rendering
- [ ] Optional cheaper fake-mirror mode
- [ ] Neutral lighting preset
- [ ] Warm lighting preset
- [ ] Daylight lighting preset
- [ ] Evening lighting preset
- [ ] Background color selection
- [ ] Dressing-room environment selection
- [ ] Idle pose
- [ ] Hands-on-hips pose
- [ ] Turn-around animation
- [ ] Walk forward/back preview
- [ ] Sit preview
- [ ] Outfit screenshot
- [ ] Before/after outfit comparison

## P6 – Outfit system

- [ ] Outfit data model
- [ ] Save outfit
- [ ] Load outfit
- [ ] Rename outfit
- [ ] Delete outfit
- [ ] Duplicate outfit
- [ ] Outfit thumbnail
- [ ] Favorite outfit
- [ ] Outfit notes
- [ ] Outfit category
- [ ] Everyday category
- [ ] Work category
- [ ] Party category
- [ ] Formal category
- [ ] Sport category
- [ ] Summer category
- [ ] Winter category
- [ ] Outfit history
- [ ] Recently worn
- [ ] Outfit randomizer
- [ ] Outfit slot conflict validation
- [ ] Jacket layering validation
- [ ] Dress layering validation
- [ ] Shoe compatibility metadata
- [ ] Accessory compatibility metadata
- [ ] Outfit share/export data structure

## P7 – Clothing layers and realism

- [ ] Body layer
- [ ] Underwear layer
- [ ] Base top layer
- [ ] Base bottom layer
- [ ] Dress layer
- [ ] Sweater layer
- [ ] Jacket layer
- [ ] Coat layer
- [ ] Shoes layer
- [ ] Accessory layer
- [ ] Layer compatibility matrix
- [ ] Dynamic body masking
- [ ] Sleeve masking
- [ ] Trouser/boot overlap handling
- [ ] Shirt tuck-in state
- [ ] Shirt untucked state
- [ ] Jacket open state
- [ ] Jacket closed state
- [ ] Zipper state
- [ ] Button state
- [ ] Hood up/down state
- [ ] Rolled sleeves state
- [ ] Cloth thickness parameter
- [ ] Layer offset parameter
- [ ] Automatic garment collision tuning
- [ ] LOD strategy for clothing
- [ ] Nanite compatibility review where relevant

## P8 – Shoes and accessories

### Shoes
- [ ] Shoe slot
- [ ] Shoe size metadata
- [ ] Sneakers
- [ ] Boots
- [ ] Heels
- [ ] Flats
- [ ] Sandals
- [ ] Shoe color/material import
- [ ] Left/right shoe pairing
- [ ] Foot morph compatibility
- [ ] Heel pose adjustment
- [ ] Shoe shelf representation

### Accessories
- [ ] Glasses
- [ ] Sunglasses
- [ ] Earrings
- [ ] Necklace
- [ ] Bracelet
- [ ] Watch
- [ ] Rings
- [ ] Scarf
- [ ] Hat
- [ ] Cap
- [ ] Bag
- [ ] Handbag
- [ ] Belt
- [ ] Accessory sockets
- [ ] Accessory physics
- [ ] Accessory collision
- [ ] Accessory wardrobe storage

## P9 – Animation

- [ ] Idle animation
- [ ] Walk animation
- [ ] Turn animation
- [ ] Look-around animation
- [ ] Wardrobe open animation
- [ ] Wardrobe close animation
- [ ] Reach for hanger animation
- [ ] Take hanger animation
- [ ] Return hanger animation
- [ ] Dressing transition
- [ ] Undressing transition
- [ ] Jacket on animation
- [ ] Jacket off animation
- [ ] Shoe change animation
- [ ] Mirror pose animations
- [ ] Hair movement
- [ ] Cloth movement
- [ ] Foot IK
- [ ] Hand IK
- [ ] Interaction IK
- [ ] Animation state machine
- [ ] Animation retargeting strategy
- [ ] Final skeleton standardization

## P10 – UI / UX

- [ ] Main HUD
- [ ] Interaction prompt
- [ ] Clothing name popup
- [ ] Clothing category icon
- [ ] Wardrobe inventory UI
- [ ] Avatar creator UI
- [ ] Hair browser
- [ ] Hair color selector
- [ ] Clothing import UI
- [ ] Import progress UI
- [ ] Import error UI
- [ ] Clothing fit adjustment UI
- [ ] Outfit browser
- [ ] Outfit save dialog
- [ ] Search UI
- [ ] Filter UI
- [ ] Favorite UI
- [ ] Settings UI
- [ ] Confirmation dialogs
- [ ] Undo last clothing change
- [ ] Redo clothing change
- [ ] Reset outfit
- [ ] Tooltips
- [ ] Keyboard shortcuts display
- [ ] Controller glyphs
- [ ] Responsive UI layouts
- [ ] 16:9 support
- [ ] 21:9 support
- [ ] 4K UI scaling
- [ ] High-DPI UI scaling

## P11 – Import pipeline / processing architecture

- [ ] Import job data model
- [ ] Import job queue
- [ ] Import state: pending
- [ ] Import state: analyzing
- [ ] Import state: generating
- [ ] Import state: fitting
- [ ] Import state: complete
- [ ] Import state: failed
- [ ] Retry failed import
- [ ] Cancel import
- [ ] Import diagnostics
- [ ] Import logs
- [ ] Cache generated clothing
- [ ] Cache generated avatar
- [ ] Duplicate-image detection
- [ ] Stable clothing asset IDs
- [ ] Stable avatar IDs
- [ ] Temporary file cleanup
- [ ] Storage quota handling
- [ ] Offline/local pipeline design
- [ ] Optional cloud-processing abstraction
- [ ] Provider-independent AI adapter interface
- [ ] Asset provenance metadata
- [ ] Rebuild generated asset from source images

## P12 – Data/privacy

- [ ] Define local storage locations
- [ ] Keep original personal photos separate from generated assets
- [ ] Delete original photos option
- [ ] Delete generated avatar option
- [ ] Delete imported clothing option
- [ ] Clear all profile data
- [ ] Export profile data
- [ ] Backup profile data
- [ ] Restore profile data
- [ ] Avoid committing personal photos to Git
- [ ] Add personal-photo paths to .gitignore
- [ ] Add generated private assets paths to .gitignore
- [ ] Privacy documentation
- [ ] Source-photo retention setting

## P13 – Audio

- [ ] Wardrobe door open sound
- [ ] Wardrobe door close sound
- [ ] Hanger movement sound
- [ ] Clothing rustle
- [ ] Drawer open/close sound
- [ ] UI hover sound
- [ ] UI select sound
- [ ] Save confirmation sound
- [ ] Dressing-room ambience
- [ ] Audio volume settings
- [ ] Master volume
- [ ] Effects volume
- [ ] Music volume

## P14 – Visual polish

- [ ] Production room
- [ ] Production wardrobe
- [ ] Production mirror
- [ ] Realistic lighting
- [ ] Lumen setup
- [ ] Reflection quality pass
- [ ] Shadow quality pass
- [ ] Character skin shader
- [ ] Eye shader
- [ ] Hair shader
- [ ] Fabric master material
- [ ] Denim material
- [ ] Knit material
- [ ] Leather material
- [ ] Silk/glossy material
- [ ] Metal accessory material
- [ ] Improved hanger model
- [ ] Wardrobe wood material
- [ ] Clothing thumbnail lighting setup
- [ ] Photo mode lighting
- [ ] Post-processing profile

## P15 – Performance

- [ ] Baseline FPS measurement
- [ ] CPU profile
- [ ] GPU profile
- [ ] Memory profile
- [ ] Avatar LODs
- [ ] Hair LODs
- [ ] Clothing LODs
- [ ] Wardrobe item LODs
- [ ] Texture streaming
- [ ] Clothing texture resolution limits
- [ ] Thumbnail texture limits
- [ ] Async asset loading
- [ ] Wardrobe visibility culling
- [ ] Cloth simulation budget
- [ ] Hair simulation budget
- [ ] Mirror rendering budget
- [ ] Performance presets Low/Medium/High/Epic
- [ ] Target 60 FPS at 1080p
- [ ] Target 60 FPS at 1440p
- [ ] Test 4K

## P16 – Testing / diagnostics

- [ ] In-game debug HUD
- [ ] Display current equipped slots
- [ ] Display focused interactable
- [ ] Display current wardrobe item IDs
- [ ] Display current avatar morph values
- [ ] Display clothing fit values
- [ ] Toggle collision visualization
- [ ] Toggle clothing clipping debug
- [ ] Toggle body masking debug
- [ ] Debug wardrobe reset
- [ ] Debug reset avatar
- [ ] Debug spawn clothing item
- [ ] Logging categories
- [ ] Wardrobe log category
- [ ] Avatar log category
- [ ] Import log category
- [ ] Save/load log category
- [ ] Crash diagnostics notes
- [ ] Automated wardrobe unit tests
- [ ] Automated save/load tests
- [ ] Automated slot conflict tests
- [ ] Automated imported-item metadata tests
- [ ] Smoke-test checklist
- [ ] Regression-test checklist

## P17 – Source control / build

- [ ] Review Unreal .gitignore
- [ ] Git LFS strategy for large assets
- [ ] Git LFS patterns for .uasset
- [ ] Git LFS patterns for .umap
- [ ] Git LFS patterns for large textures
- [ ] Branch strategy
- [ ] Development branch
- [ ] Stable/main policy
- [ ] Commit naming convention
- [ ] Version numbering
- [ ] Build script
- [ ] Clean build script
- [ ] Launch editor script
- [ ] Package development build script
- [ ] Package shipping build script
- [ ] Build log folder
- [ ] Build diagnostics
- [ ] GitHub Actions compile check
- [ ] GitHub Actions artifact build
- [ ] Release notes template
- [ ] Tagged releases

## P18 – Future ideas

- [ ] Multiple avatars
- [ ] Male avatar support
- [ ] Child avatar support
- [ ] Multiple body bases
- [ ] Shared household wardrobe
- [ ] Wardrobe rooms
- [ ] Walk-in closet
- [ ] Multiple wardrobes
- [ ] Seasonal wardrobe
- [ ] Packing list mode
- [ ] Holiday outfit planning
- [ ] Calendar outfit planning
- [ ] Outfit history by date
- [ ] Weather-aware outfit suggestions
- [ ] Color matching assistant
- [ ] Outfit recommendation engine
- [ ] "What should I wear?" mode
- [ ] Clothing usage statistics
- [ ] Least-worn clothing view
- [ ] Favorite combinations
- [ ] Outfit comparison grid
- [ ] Virtual photo shoot
- [ ] Pose library
- [ ] Room customization
- [ ] Share outfit screenshot
- [ ] Mobile companion concept
- [ ] Webcam/camera capture concept
- [ ] AR try-on research
- [ ] VR wardrobe research

## Immediate next build queue

1. Interaction prompt UI.
2. Clothing name/category popup.
3. Prevent hanger interaction while wardrobe doors are closed.
4. Add dedicated trousers + shoes placeholder items.
5. Add production-style hanger geometry.
6. Smooth wardrobe door animation.
7. Improve dummy proportions.
8. Add underwear/base layer representation.
9. Add first hair placeholder and hair-color UI.
10. Add outfit browser with multiple save slots.
11. Add basic debug HUD.
12. Add Git LFS configuration before binary assets begin growing.
13. Add first rigged test garment.
14. Add first actual humanoid mannequin replacement for the cube dummy.
15. Start clothing import screen shell.

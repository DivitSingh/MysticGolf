# Mystic Golf Game Overview
A procedurally-generated mystical minigolf experience featuring:
- Procedurally-generated levels
- Physics-based golf mechanics (Shot Power and Angle Control, Collisions etc.)
- Mystical elements and obstacles (Cyclic Whirlpools, Accelerating Runes, Holy Water, and 
Occultation Towers)

The player must steer their Divine Ball through the ritual grounds, destroying all Occultation 
Towers in order to reveal the location of the Summoning Circle. Once found, the ball must be 
guided into the circle to end the profane ritual. Obstacles such as Whirlpools will suck or repel 
the ball cyclically, as well Runes that speed up the ball when it comes into contact. Holy Water 
can be collected and activated at will to empower the ball into its golden state that makes it 
immune to the nefarious effects of the Whirlpools.

I took a spin on the minigolf formula, replacing hazards like sand and water with Runes and 
Whirlpools, adding new elements such as Occultation Towers and Summoning Circles to really 
lean into the mystical aspect of the game.

# Core Gameplay Elements

## Level Generation
- Unique layout each playthrough
- Strategic element placement algorithm to prevent 'impossible' levels
- Balanced difficulty scaling
- Safe zones around starting positions to prevent awkward starts

## Golf Mechanics
- Power and angle-based shots
- Physics-driven ball movement
- Shot counter tracking

## Mystical Elements
- Occultation Towers that conceal the Summoning Circle (hole)
- Magical Whirlpools creating cyclically-changing force fields
- Ancient Runes providing acceleration zones
- Holy Water (Powerup) granting temporary immunity to whirlpools

## Technical Features
- Procedural content generation
- Real-time physics calculations
- Dynamic collision systems
- Audio-visual feedback systems (event-based animations and audio queues)
- Optimized code to prevent redundant checks with early returns, cached calculations etc.

# Level Generation (```LevelGenerator``` Class)

## Algorithm Overview
The level generation system employs a multi-phase approach combining cellular automata concepts
with constraint-based placement algorithms.

### Generation Phases

#### Grid Initialization
- Creates a 16x12 cell grid representation
- Establishes boundary walls
- Implements corner-based spawn point selection
- Sets up data structures for path validation

#### Path Generation
The system employs pathfinding constraints to ensure playability:
- Guarantees traversable paths between start and end points
- Validates cell connectivity
- Maintains minimum path widths
- Creates strategic chokepoints

#### Element Distribution
Implements sophisticated placement algorithms:
- Proximity-based distribution using Euclidean distance calculations
- Safe zone radius of 128 pixels around spawn points
- Element density control through probability matrices
- Strategic element clustering prevention

#### Constraint Handling
The system manages multiple placement rules:
- Minimum 2 towers per level (guaranteed challenge)
- Maximum element counts per type
- Proximity restrictions between elements
- Path adjacency requirements

### Look Here in Code
```cpp
void LevelGenerator::PlaceMechanics() {
    // Key code snippets demonstrating:
    - Distribution algorithms
    - Constraint checking
    - Safe zone calculations
}
```

# Golf Mechanics

## Shot Mechanics (```GolfBall``` and ```AimArrow``` Classes)

### Power System
- Linear power scaling (1.0f to 10.0f)
- Visual power indicator (Aim Arrow size changes)
- Precise control through input mapping
- Velocity calculation: velocity = power * direction_vector

### Angle Calculation
Indicated via Aim Arrow direction.
```cpp
float angleRad = angle * PI / 180.0f;
velocity_x = power * cosf(angleRad);
velocity_y = power * sinf(angleRad);
```

### Movement Physics
- Velocity-based updates
- Frame-independent movement
- Friction application per frame
- State tracking for shot completion

## Input Integration (```Game.cpp``` ```ProcessInput()``` Function)
The system handles multiple input methods:

### Keyboard Controls
- Horizontal Arrow Keys for Shot Aim
- Verical Arrow Keys for Shot Power
- Spacebar for Shot Execution
- Shift Key for Powerup Activation

### Controller Support
- Left Analog Stick for Shot Aim
- Right Analog Stick for Shot Power
- 'A'/South Button for Shot Execution
- 'X'/West Button for Powerup Activation

## Technical Optimizations
- Efficient collision response: (```Wall``` Class and ```Game.cpp``` ```CheckCollisions()``` Function)
    - Early-out checks for performance
    - Efficient radius calculations
    - Minimal square root operations
    - Optimized boundary checks
- Smooth movement interpolation
    - Pre-calculated constants
    - Minimized trigonometric operations
    - Efficient vector operations
    - Cached calculations where beneficial
- Input Processing
    - Deadzone handling for precise control
    - Input buffering for responsive feedback
    - Efficient state transitions
    - Optimized input polling

# Mystical Elements

## Occultation Towers (```Tower``` Class)
All towers present in the level must be destroyed in order to reveal the summoning circle, i.e.,
the hole where the ball must be aimed.
On destruction, the tower has an explosion animation and sound that plays, before it changes its
appearance to a pile of rubble.

### Technical Highlights
- State-driven animation system
- Radius-based collision detection
- Event-driven hole visibility control
- Memory-efficient sprite management

### Look Here in Code
```cpp
class Tower {
    // Key code snippets demonstrating:
    - Sprite state management (functional/destroyed)
    - Collision detection system
    - Destruction animation sequence
    - Visibility control of the summoning circle based on how many towers left
```

## Cyclic Whirlpools (```Whirlpool``` Class)
Implements a force field system using vector calculations. The whirlpools alternate between having
an attracting or repelling effect on the nearby area, indicated by their color. The player can plan
accordingly to get a huge boost to their speed if they time their shot correctly.

### Technical Highlights
- Dynamic force application
- Circular motion calculations
- Frame-based animation system
- Efficient collision checking

### Look Here in Code
```cpp
class Whirlpool {
    // Physics Implementation:
    - Force vector calculations
    - Rotational velocity application
    - Animation frame management (changes based on current cycle)
    - Radius-based effect zones
```

## Ancient Runes (```Rune``` Class)
Acceleration zones that boost the ball's speed when it goes through them.

### Technical Highlights
- Vector-based movement enhancement
- Directional force application
- Optimized collision detection
- State-driven animations

### Look Here in Code
```cpp
class Rune {
    // Velocity System:
    - Direction-based acceleration
    - Speed multiplication
    - Collision detection
    - Visual feedback system
```

## Holy Water (```Powerup``` Class)
Provides temporary immunity from the repulsive and attrative forces of the Cyclic
Whirlpools. The amount in inventory and active immunity duration (when the powerup is consumed) are
displayed on-screen. They can be collected and stored to be used later.

### Technical Highlights
- Timer-based state management
- Collection radius calculations
- Effect application system
- Memory-efficient powerup pooling

### Look Here in Code
```cpp
class PowerupManager {
    // Core Systems
    - Collection detection
    - Duration management
    - State tracking
    - Visual effects
```
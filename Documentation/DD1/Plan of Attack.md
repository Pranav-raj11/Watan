
## 1. Models

> Models are primarily equipped with methods for getters. If you want to set, the ModelFacade returns a reference to the model you desire to change


- **Class:** `ModelFacade` - this class is just an interface for all classes that want to interact with the models. Mostly just calls the other model instances
	- **Methods**: `getBoard()` and `getPlayer()`

- **Class**: `GenericModel`
  - **Method**: `getBoard()`
    - Return a reference to the current game board.
  - **Method**: `getStudent(color: enum): Student &`
    - Retrieve a `Student` object based on the specified player color.

- **Class**: `Board`
  - **Method**: `getAdjacentGoalsAndCriterions(int objectiveNum, bool objectiveNumIsGoal): vector<Objective*> &`
    - Retrieve adjacent goals and criterions for a given objective.

- **Class**: `HexTile`
  - **Attributes**: `tileResource`, `goals`, `criterions`
  - **Method**: `getTileResource(): Resource`
    - Retrieve the resource type of the hex tile.
  - **Method**: `awardCriterions()`
    - Award resources to players based on nearby Criterions.

- **Class**: `Player`
  - **Method**: `incrementVP(int amount)`
    - Increase the player's victory points.
  - **Method**: `incrementCaffeine(int amount)`
    - Increase the player's caffeine resource count.
  - **Method**: `incrementLectures(int amount)`
    - Increase the player's lectures resource count.
  - **Method**: `incrementTut(int amount)`
    - Increase the player's tutorial resource count.
  - **Method**: `incrementStudy(int amount)`
    - Increase the player's study resource count.
  - **Method**: `spendResources(enum resourceType, int amount)`
    - Deduct specified resources for actions.
  - **Method**: `getNumVictoryPoints(): int`
    - Return the total number of victory points.
  - **Method**: `getCurrentDice(): enum`
    - Retrieve the current dice type for the player.

- **Class**: `Objective`
  - **Method**: `getObjectiveNumber(): int`
    - Return the number associated with this objective.
  - **Method**: `getStudentWhoCompleted(): Student*`
    - Retrieve the student who completed this objective.

- **Class**: `Goal` 
  - **Method**: `getType(): string`
    - Return the type of the goal.

- **Class**: `Criterion` 
  - **Method**: `getType(): string`
    - Return the type of the criterion.
  - **Method**: `getCompletionLevel(): enum`
    - Retrieve the current completion level of the criterion.

- **Class**: `Dice`
  - **Method**: `getType(): string`
    - Return the type of the dice (e.g., Fair or Deterministic)
  - **Method**: `roll(): int`
    - Roll the dice and return the result.

- **Class**: `FairDice` (Inherits from `Dice`)
  - **Method**: `getType(): string`
    - Return the type of the dice ("Fair").
  - **Method**: `roll(): int`
    - Implement a fair dice roll.

- **Class**: `DeterministicDice` (Inherits from `Dice`)
  - **Method**: `getType(): string`
    - Return the type of the dice ("Deterministic").
  - **Method**: `roll(): int`
    - Return a predetermined number for testing purposes.

- **Class**: `Student`
  - **Method**: `getType(): string`
    - Return the type of student (eg: AggressiveComputer, Computer, Student)




## 2. Set Random Seed

- **Class**: `RandomGenerator`
  - **Method**: `constructor`
    - Take in a seed and set the random generator seed to be the provided integer
- **Function:** `main`
	- **Task:** take the provided seed provided in `argv` and pass this to `GameController` to set the seed
## 3. Printing the Board
- **Class**: `GameView`
  - **Method**: `displayBoard()`
    - Display the current state of the board, including all Criterions and Goals (with the player colors, if completed)
    - Includes resource provided in each tile
    - Includes the tile number and the tile value in each tile

## 2. Game Initialization (New Game, Setting Board)

- **Class**: `GameController`
  - **Method**: `startGame()`
	  - Call the facade's initialize game function to start a new game
	  - Create a game loop to run the game
- **Class:** `ManagerFacade`
	- **Method**: `initializeNewGame()`
		- Calls appropriate methods in each concrete manager to initialize the game 
- **Class**: `GameStateManager`
	- **Method**: `initializeNewGame()`
		- Generates random resources and value associated to the hex tiles
		- Calls `BoardManager`'s `initializeBoard()` to set the board
- **Class:** `BoardManager`:
	- **Method:** `initializeBoard(vector<int> tileValues, vector<int> tileResources)`
		- Takes provded tile value and resources to create the board with 19 tiles. Interacts with the Model Facade

## 3. Players Choose Initial Assignments

- **Class**: `GameController`
  - **Method**: `getInitialAssignments()`
	  - Ensure the players choose an initial assignment by calling the `GameView`'s `chooseInitialAssignmentsToComplete()` for each student. Then use the manager's facade to complete the assignments / achieve the goal
## 4. Loading a Game (or Loading a Board)

- **Class:** `GameStateManager`
	- Implement the `loadGame()` function that loads a game, given a filename
	- Implement the `loadBoard()` function that loads a game, given a filename for the saved board
	- Directly interact with `BoardManager`, `ObjectiveManager`, and `ResourceManager` while setting the board / criterions / resources. DO NOT DIRECTLY INTERACT WITH THE MODEL FACADE WITH THIS CLASS
## 5. Saving a Game

- **Class:** `GameStateManager`
	- **Methods:** `saveGame()`
		- Read from the `ModelFacade` to obtain relevant data, and save it as described in the notion file
## 6. Main Game Loop

- **Class**: `GameController`
  - **Methods**: implement the methods as provided in the UML
- **Task:** GameController handles the flow of the game. This includes the game loop, deciding which classes to delegate a task to, etc.
	- In here, make a loop to iterate through players' turn one at a time

## 7. End of Turn Options

- **End of Turn Options:** provide options at the end of each player's turn to run an action.
- **Class:** `GameView`
	- **Methods:** `promptEndOfTurnOptions()`
		- Prompt the player to enter an action. Return the chosen action once called
- **Class:** `GameController`
	- **Methods:** `onCompleteCriterion()`, `onTradeRequest()`, ... (every private function that deals with the actions available at the end of the turn)
	- Users should be able to choose an action; the action should be executed

## 8. Obtaining Resources

- Add functionality so players can obtain resources when a non-7 number is rolled
- **Class:** `HexTile`
	- **Methods:** `awardCriterions()`
		- This method will award all users with a criterion in the provided Hex Tile with the resource in the tile, depending on the level of completion
- **Class:** `ManagerFacade`
	- **Methods:** `awardResourcesOnRoll()`
		- This method will award users rewards, depending on the roll of the dice. **Remember to ensure that ALL tile values are given the reward**


## 9. Criterion Completion + Upgrading, Goal Achievements

- **Class**: `Criterion`
  - **Method**: `complete()`
    - Allow a player to achieve or complete a Criterion.
- **Class**: `ResourceManager`
  - **Method**: `completeCriterion()`, `improveCriterion()`, `achieveGoal()`
    - Handles the logic of interacting with the ModelFacade to complete / improve a criterion. Throws an error indicating the type of failure if it occurs (eg: NotEnoughResources, NotValidLocation)
## 10. Trading Resources

- **Class:** `ManagerFacade`
	- **Method:** `tradeResources()`
		- Just call on `ObjectiveManager`'s public functions to run the trade
		- **ENSURE THAT THE USER ACTUALLY HAS ENOUGH RESOURCES TO TRADE**
- **Class:** `GameView`
	- **Methods:** `tradeSequence()`
		- Provides the sequence of prompts required to create a trade request. Returns a boolean indicating whether a player accepted the trade or not
    

## 11. Rolling a Geese - Moving, Players Losing Resources, Stealing

- **Class**: `GameView`
	- **Methods:** `showLostResourcesOnGeeseRolled()`, `promptNewGeeseLocation()`, `promptStealResources()`, `promptStolenResources()`
		- Add the prompting functionalities so the user can choose where to place the geese and who (if possible) the player wants to steal a random resource from
- **Class:** `BoardManager`
	- **Method:** `getStudentsToStealFrom()`
		- Returns a set of possible options, which the student can steal from based on available resources and the new geese tile location
## 12. End of Game

- Determine the winner and produce the output using `GameView`
- Provide an option to play again

## 13. Unit Tests

> This should be done continuously while implementing each feature. Create a test harness for it and develop test cases to ensure validity



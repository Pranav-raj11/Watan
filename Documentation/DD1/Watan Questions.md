**Question 1**: You have to implement the ability to choose between randomly setting up the resources of the board and reading the resources used from a file at runtime. What design pattern could you use to implement this feature? Did you use this design pattern? Why or why not?

To decide between randomly setting up the resources against loading a configuration from a file, we would use the Template method pattern. In both cases, we have a common recipe: to initialize a game. So we can have an abstract base class `GameLoader` with a pure virtual method `loadGame()` and `loadGame(string filename)`. Then we could have two concrete subclasses, `PreviousGameLoader` and `NewGameLoader` that implement the `loadGame()` function.

We did not use this pattern. The core logic of our program (i.e the entire program as a whole) is built using the MVC architecture- models to track the game state, controllers to decide the flow of the game, and views to display the states and prompting the users for actions. We decided to create a concrete controller class that manages saving and loading games. This way, we avoid deeply nested class hierarchies. This concrete controller has two methods: one to initialize a new game and another to load an existing game. In either case, it has to interact with the models to load a game state. We decided to have a single class resposible for this rather than having multiple, separate concrete classes (as in the factory method) to develop these features. This is so that it is easier to track down any bugs (as opposed to having a larger class hierarchy) and also easier to use the class.

Moreover, using the factory method pattern would require having these classes interact with the models. We did not want this- the only classes that should modify the models are the controllers. Hence we developed a concrete controller that manages saving and loading a game. More precisely, we created a set of "Manager" classes that the primary `Controller` class delegates its work to. The `Manager` classes contain logic for interactions with the `Model` classes. So we developed a concrete `GameStateManager` that handles saving and loading a game (or a board). 

**Question 2**: You must be able to switch between loaded and unloaded dice at run-time. What design pattern could you use to implement this feature? Did you use this design pattern? Why or why not?

We can use the factory design pattern to switch between loaded and unloaded dice at run-time. For instance, we can have a base Dice class like the following:

```cpp
class Dice {
	public:
		int roll() = 0;
}
```

Then we can have two concrete classes, `FairDice` and `DeterministicDice` that implement the concrete roll() methods
```cpp

class FairDice: public Dice {
	public:
		int roll() override {
		// choose a random number and reurn
		}
}

```

```cpp
class DeterministicDice: public Dice {
	int myNum;
	public:
		int roll() override {
			// initialize myNum in ctor
			return myNum;
		}
}
```

With this, anytime the player decides to change their dice, we can construct a new `Dice` object with the concrete implementation chosen by user. After that, determining the value of the roll is as simple as calling `roll()`.

We did use this solution. All dice have a common functionality, which is to produce the output of `roll()`. Using a `Factory` class with the abstract `Dice`, we can produce any types of dice during run-time. With this approach, we can extend the functionality of the Dice feature. For example, we can implement additional sorts of dice without having to modify existing code- we just create it in the `Factory` class.

In addition, the code is easier to maintain as each dice is encapsulated in its own class. Hence debugging related tasks will be much simpler to resolve.

**Question 4**: At the moment, all Watan players are humans. However, it would be nice to be able to allow a human player to quit and be replaced by a computer player. If we wanted to ensure that all player types always followed a legal sequence of actions during their turn, what design pattern would you use? Explain your choice

The Proxy design pattern is highly suitable for this case. For our program, we have decided to use the MVC pattern for the core logic of the game (i.e the controller will manage the game logic, the models will contain the state of the game, and the view will handle I/O). In our case, we can have a Proxy Interface between the Controller and the view. Every time the controller calls a method in the view to prompt the user to enter an action (eg: roll a dice), there is a possibility that the user enters an invalid value (eg: a string "NO"). In this case, we can have a Proxy interface that handles invalid inputs. The proxy will reprompt the user until the user enters a valid response. The proxy then returns this to the controller.

We chose the proxy pattern because this interface can be built without the need to modify the View and Controller classes. It will be easy to add additional checks in the proxy, and modify the View class without much overhead. Handling invalid inputs and getting the input are treated as separate tasks, which makes extension for either easier.

If we did have to implement invalid input handling directly in the `View` class, this would make it harder to add additional checks for input handling. For example, if we had to check whether an input is a valid integer in the current version, but later decided that the user can also enter strings such as "one", then adding this would be much harder as we have to modify the functionality of the `View` class. On the other hand, if we had a proxy interface managing this, adding new features for input handling would be much easier to implement and debug.

**Question 5**: What design pattern would you use to allow the dynamic change of computer players, so that your game could support computer players that used different strategies, and were progressively more advanced/smarter/aggressive?

This feature is *highly* suitable with the factory method pattern. This way, we have a base `ComputerPlayer` class that inherits from the base `Student` class (the `Student` class would be an ABC for all players)
```cpp
class ComputerPlayer: public Student {
	public:
		int getMove(/* pass in current data */) = 0;
}
```
And then we can create concrete implementations of `ComputerPlayer`, with different play styles (eg: more aggressive) or smarter"
```cpp
class DumbComputerPlayer: public ComputerPlayer {
	public:
		int getMove(/* some data */) override {
			// make decision and return the move
		}
}

class SmartComputerPlayer: public ComputerPlayer {
	public:
		int getMove(/* some data */) override {
			// make decision and return the move
		}
}
```

And we can create a `Factory` class to produce these players. This makes adding additional sorts of `ComputerPlayers` much easier since we do not need to modify existing implementations of other parts of the system. We just need to modify the `Factory` class to create the new players.

With this approach, the decision-making logic of the `ComputerPlayer` is encapsulated within their respective class. In addition, the creation of these `ComputerPlayer`s is encapsulated in the `Factory` class. This provides the ability to dynamically change the *real* players to `ComputerPlayer`s during runtime within the `Factory` class, while abstracting this to the `Factory` class.

In addition, centralizing the creation of the `ComputerPlayer`s to the `Factory` class makes the codebase much easier to debug. Separating the instantiation of the classes and the implementation of the players' logic will enable us to pinpoint any bugs related to computer players with ease.

**Question 6**: Suppose we wanted to add a feature to change the tiles’ production once the game has begun. For example, being able to improve a tile so that multiple types of resources can be obtained from the tile, or reduce the quantity of resources produced by the tile over time. What design pattern(s) could you use to facilitate this ability?

To add the ability to dynamically change the tiles' production, we would use the visitor pattern. We would include an `accept` method in the Tile class

```cpp
class Tile {
	public:
		// other methods...
		// accept a visitor
		void accept(Visitor & visitor) {
			// call the visitor's traverse method
			visitor.visit(this);
		}
}
```

And then the `Visitor` class can modify the properties of the tile (such as production rate, number of resources, etc.). The visitor pattern enables adding additional functionality without modifying the implementation of the tile. For instance, during the first iteration of this system, the `Tile` class may have a `string` to store the type of resource on the tile. Now if we wanted to offer multiple resources, we would have to modify the implementation of tile, and other classes (such as the `GameView` class in our case, which is responsible for I/O operations. This means the `GameView` class will need to get all rewards a user received, so these can be printed out in `cout`). Refactoring the code to change the `string` to a `vector` to store multiple resources can be a hassle and lead to many bugs.

However, with the visitor design pattern, we can add this functionality without changing any implementations of the `Tile` class. We can develop a concrete `ResourceVisitor` that can visit the `Tile` and award players with multiple (or fewer) resources. This also makes extension to this system much easier. For example, if we wanted to implement a feature where we wanted to change the resources offered on a tile based on the player who is rolling the dice, then there is no need to modify the implementations of the `Tile` class. We can either make another concrete visitor class or modify the existing `ResourceVisitor` to handle this.

Moreover, pinpointing any bugs in this system will be far easier. The `ResourceVisitor` is specifically responsible for distributing additional resources to the players; so any issues will likely originate from this class. 
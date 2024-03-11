## Class Description
The CTransportationPlannerCommandLine class offers a command-line interface (CLI) for interacting with the CTransportationPlanner system. It facilitates user interactions by allowing the execution of various commands related to transportation planning, such as querying node information, finding shortest or fastest paths, and managing input and output operations.

## Constructor and Destructor
CTransportationPlannerCommandLine Constructor
Purpose: Constructs a CTransportationPlannerCommandLine instance, setting up the necessary components for command-line interactions with the transportation planner.

Parameters:

std::shared_ptr<CDataSource> cmdsrc: A shared pointer to a command data source, used to read commands.
std::shared_ptr<CDataSink> outsink: A shared pointer to an output data sink, used for standard command outputs.
std::shared_ptr<CDataSink> errsink: A shared pointer to an error data sink, used for error messages.
std::shared_ptr<CDataFactory> results: A shared pointer to a data factory, used for processing and handling results.
std::shared_ptr<CTransportationPlanner> planner: A shared pointer to the transportation planner, the core component for planning operations.
CTransportationPlannerCommandLine Destructor
Purpose: Destroys the CTransportationPlannerCommandLine instance, performing necessary cleanup of resources.

## Key Methods

ProcessCommands
Purpose: Starts processing commands from the command source, facilitating user interactions until an "exit" command is encountered.

Parameters: None

Return Type: bool

Indicates whether command processing was successful.

## ParseCommandLine
Purpose: Parses a given line of command text into a vector of tokens for easier handling.

Parameters:

const std::string& line: The command line text to be parsed.
Return Type: std::vector<std::string>

Returns a vector of string tokens derived from the command line.

## HandleCommand

Purpose: Processes and executes a command based on the parsed tokens.

Parameters:

const std::vector<std::string>& tokens: A vector of tokens representing a command and its arguments.
Return Type: None

## Detailed Handling of Commands

help: Displays help information.
exit: Exits the command-line interface.
count: Shows the total number of nodes.
node <index>: Retrieves and displays information about a node by its index.
fastest <start node ID> <end node ID>: Finds and displays the fastest path between two nodes.
shortest <start node ID> <end node ID>: Finds and displays the shortest path between two nodes.
save: Saves the last calculated path to a file.
print: Prints details of the last calculated path.

## Example Usage

Assuming the system is set up with appropriate data sources and sinks:


int main() {
    // Initialization of data sources, sinks, and the planner
    auto cmdSrc = std::make_shared<CDataSource>(/* parameters */);
    auto outSink = std::make_shared<CDataSink>(/* parameters */);
    auto errSink = std::make_shared<CDataSink>(/* parameters */);
    auto results = std::make_shared<CDataFactory>(/* parameters */);
    auto planner = std::make_shared<CTransportationPlanner>(/* parameters */);

    CTransportationPlannerCommandLine cli(cmdSrc, outSink, errSink, results, planner);
    cli.ProcessCommands(); // Starts processing commands from the command source

    return 0;
}
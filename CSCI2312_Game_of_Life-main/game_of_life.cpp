#include <iostream>
#include <chrono>
#include <thread>
#include <string>

// Info for Sleep function:
// https://stackoverflow.com/questions/158585/how-do-you-add-a-timed-delay-to-a-c-program
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;
// Initializes and sets boundaries
const int WIDTH = 50;
const int HEIGHT = 20;
// Probability of each cell starting out as alive
const float init_probability_alive = 0.5;

// W => Windows
// M => Mac
// L => Linux
const char SYSTEM = 'W';
// Basic cell
struct Cell{
    int sum = 0;
    bool alive = false;
    bool will_switch = false;
};
// Clears console depending on which character is entered
void clear_console(){
    // Determines what each specific key pressed does
    switch(SYSTEM){
        case 'W':
            system("cls");
            break;
        case 'M':
        case 'L':
            std::cout << "\x1B[2J\x1B[H";
            break;
        default:
            break;
    }
}
// Checks to see if an individual "cell" is alive
bool is_init_alive(){
    // Get a random value between 0 and 1
    double r = ((double) rand() / (RAND_MAX));
    // Checks if r is less than the float set as 0.5, if true returns true
    if (r < init_probability_alive){
        return true;
    }
    // There's no else statement here, will this still work?
    return false;
}
// Initializes the game, determines starting alive cells
void init_game(Cell cell[HEIGHT][WIDTH]){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            bool is_alive = false;
            if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){
                is_alive = is_init_alive();
            }
            cell[i][j].alive = is_alive;
        }
    }
}

int main() {
    // Asks user to enter a number
    std::cout << "game start, type a number" << std::endl;
    // Initializing int and char
    int temp_seed;
    char temp;
    // Saves user entered number to temp_seed
    std::cin >> temp_seed;
    // Generates seed for rand() function using number entered by user
    srand(temp_seed);
    unsigned int round_count = 1;
    // Initializes a version of Cell
    Cell cell[HEIGHT][WIDTH];
    // Initializes game using Cell cell
    init_game(cell);
    // Sets game_running to true, starts game
    bool game_running = true;
    // Allow user to exit after x rounds
    int round_pause = 200;
    // Loops as long as game_running = true
    while(game_running){
        // Draw the grid
        bool all_dead = true;
        for(int i = 0; i < HEIGHT; i++){
            for(int j = 0; j < WIDTH; j++){
                if(cell[i][j].alive){
                    all_dead = false;
                }
                if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){
                    int sum = 0;
                    for(int k = -1; k <= 1; k++) {
                        for (int m = -1; m <= 1; m++) {
                            // Check to make sure [i][j] isn't self
                            if(k == 0 && m ==0){
                                continue;
                            }
                            if(cell[i+k][j+m].alive){
                                sum += 1;
                            }
                        }
                    }
                    cell[i][j].sum = sum;
                    if(cell[i][j].alive){
                        if(sum > 3 || sum < 2){
                            cell[i][j].will_switch = true;
                        }
                    }else{
                        if(sum == 3){
                            cell[i][j].will_switch = true;
                        }
                    }
                }
            }
        }

        std::string curr_line;
        // Determines whether a cell will switch or not
        for(int i = 0; i < HEIGHT; i++){
            for(int j = 0; j < WIDTH; j++){
                if(cell[i][j].alive){
                    curr_line += "0 ";
//                    std::cout << cell[i][j].sum << " ";
                }else{
                    curr_line += ". ";
                }
                if (cell[i][j].will_switch){
                    cell[i][j].alive = !cell[i][j].alive;
                    cell[i][j].will_switch = false;
                }
            }
            curr_line += '\n';
        }

        clear_console();
//        sleep_until(system_clock::now() + 50ms);
        std::cout << curr_line << std::endl;

        sleep_until(system_clock::now() + 50ms);
        // checks if all cells are dead
        if(all_dead){
            // Prints to console
            std::cout << "All life has been exterminated. Good job, hero." << std::endl;
            // Asks user to continue
            printf("It survived for %d rounds. Continue? Y/N",round_count);
            // Saves user answer as temp
            std::cin >> temp;
            // Determines if user wants to quit
            if(temp == 'N' || temp == 'n'){
                // Prints if user enters "N" or "n"
                printf("Good call. See ya.");
                // Ends game
                game_running = false;
            // If user enters anything else, starts game
            }else{
                init_game(cell);
            }
        }
        // Pauses game
        if(round_count % round_pause == 0){
            // Prints to console asking for user answer
            printf("Paused at %d rounds. Enter n to escape, or anything else to continue.",round_count);
            // saves user answer as temp
            std::cin >> temp;
            // Determines if user wants to quit
            if(temp == 'N' || temp == 'n'){
                // Prints to console if user enters "N" or 'n'
                printf("Good call. See ya.\n");
                game_running = false;
            }
            // Changed to an else statement as the console asks for the user to enter anything to continue
            // Original forced user to enter "R" or "r"
            else{
                // Continues game
                init_game(cell);
            }
        }
        // adds 1 ro round_count
        round_count++;
//        std::cout << rand() << std::endl;
    }
    // ends program
    return 0;
}

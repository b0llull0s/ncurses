#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>  // for usleep()

int main() {
    // Initialize ncurses
    initscr();
    curs_set(0);  // Hide cursor
    start_color();
    noecho();     // Don't print pressed keys
    nodelay(stdscr, TRUE);  // Non-blocking input

    // Define color pairs 
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);

    // Get terminal dimensions
    int width, height;
    getmaxyx(stdscr, height, width);
    int size = width * height;

    // Flame buffer (initialized to 0)
    std::vector<int> buffer(size + width + 1, 0);

    // Flame chars 
    const char* chars = " .:^*xsS#$";

    // Seed random number generator
    srand(time(nullptr));

    // Main loop
    while (true) {
        for (int i = 0; i < width / 9; i++) {
            int pos = (rand() % width) + width * (height - 1);
            buffer[pos] = 65;
        }

        // Update flame physics
        for (int i = 0; i < size; i++) {
            buffer[i] = (buffer[i] + buffer[i + 1] + buffer[i + width] + buffer[i + width + 1]) / 4;

            // Determine color and character
            int color;
            if (buffer[i] > 15) color = 4;
            else if (buffer[i] > 9) color = 3;
            else if (buffer[i] > 4) color = 2;
            else color = 1;

            char c = chars[buffer[i] > 9 ? 9 : buffer[i]];

            // Draw only if inside screen bounds
            if (i < size - 1) {
                attron(COLOR_PAIR(color) | A_BOLD);
                mvaddch(i / width, i % width, c);
                attroff(COLOR_PAIR(color) | A_BOLD);
            }
        }

        refresh();
        usleep(30000);  // 30ms delay 

        // Exit on any key press
        if (getch() != ERR) break;
    }

    endwin();  
    return 0;
}
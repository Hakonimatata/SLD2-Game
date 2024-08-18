#include "Button.hpp"



void renderButton(SDL_Renderer* renderer, Button& button) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rød farge
    SDL_RenderFillRect(renderer, &button.rect);
}


void handleButtonClick(int x, int y, std::vector<Button>& buttons) {
    // Iterer over alle knappene og sjekk om klikket var innenfor en av dem
    for (Button& button : buttons) {
        if (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
            y >= button.rect.y && y <= button.rect.y + button.rect.h) {
            button.execute(); // Kall funksjonen til knappen som ble trykket
            std::cout << "button clicked" << std::endl;
        }
    }
}
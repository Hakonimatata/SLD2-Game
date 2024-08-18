#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <functional>

// Struktur for en knapp
struct Button {
    SDL_Rect rect;                            // Knappens område
    std::function<void()> onClick;            // Funksjonsobjekt som kan holde hvilken som helst callable

    void execute() {
        if (onClick) {
            onClick();                        // Kall den bundne funksjonen
        }
        else 
        {
            std::cout << "No action bound to this button." << std::endl;
        }
    }
};

// Funksjon for å legge til en callback til en knapp
template <typename T>
void addCallback(Button& button, T* instance, void (T::*func)()) {
    button.onClick = std::bind(func, instance);
}

void handleButtonClick(int x, int y, std::vector<Button>& buttons);

void renderButton(SDL_Renderer* renderer, Button& button);




#endif // BUTTON_HPP
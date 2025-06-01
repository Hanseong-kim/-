// gui.cc
#include "gui.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <cmath>

// renderer
renderer::renderer(SDL_Renderer* r) : sdl_renderer(r) {}

renderer* renderer::subrenderer(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const {
    SDL_Rect viewport = { (int)x, (int)y, (int)width, (int)height };
    SDL_RenderSetViewport(sdl_renderer, &viewport);
    return new renderer(sdl_renderer);
}

// checkered
checkered::checkered() : toggled(false) {}

void checkered::render(renderer const& r) {
    const int square_size = 20;
    for (int y = 0; y < 200; y += square_size) {
        for (int x = 0; x < 200; x += square_size) {
            bool dark = ((x / square_size + y / square_size) % 2 == 0);
            if (toggled) dark = !dark;
            SDL_SetRenderDrawColor(r.sdl_renderer, dark ? 30 : 220, dark ? 30 : 220, dark ? 30 : 220, 255);
            SDL_Rect square = {x, y, square_size, square_size};
            SDL_RenderFillRect(r.sdl_renderer, &square);
        }
    }
}

void checkered::on_click(renderer const&, unsigned int, unsigned int) {
    toggled = !toggled;
}

void checkered::on_drag(renderer const&, unsigned int, unsigned int, int, int) {}

bool checkered::needs_update() const { return true; }

// text
text::text(SDL_Renderer* renderer, std::string txt, int font_size,
           float red, float green, float blue,
           float bg_red, float bg_green, float bg_blue)
    : sdl_renderer(renderer), current_text(txt), dirty(true)
{
    text_color = {
        Uint8(std::clamp(red * 255.0f, 0.0f, 255.0f)),
        Uint8(std::clamp(green * 255.0f, 0.0f, 255.0f)),
        Uint8(std::clamp(blue * 255.0f, 0.0f, 255.0f)), 255
    };
    bg_color = {
        Uint8(std::clamp(bg_red * 255.0f, 0.0f, 255.0f)),
        Uint8(std::clamp(bg_green * 255.0f, 0.0f, 255.0f)),
        Uint8(std::clamp(bg_blue * 255.0f, 0.0f, 255.0f)), 255
    };

    font = TTF_OpenFont("FreeSans.ttf", font_size);
    if (!font) {
    SDL_Log("❌ TTF_OpenFont failed: %s", TTF_GetError());
    } else {
    SDL_Log("✅ TTF_OpenFont succeeded.");
    }

}


void text::set_text(std::string new_text) {
    current_text = new_text;
    dirty = true;
}

void text::render(renderer const& r) {
    if (!dirty || !font) return;

    SDL_Surface* surface = TTF_RenderText_Solid(font, current_text.c_str(), text_color);
    if (!surface) {
        SDL_Log("❌ TTF_RenderText_Solid failed: %s", TTF_GetError());
        return;
    }

    SDL_SetRenderDrawColor(r.sdl_renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    //SDL_Rect bg_rect = {0, 0, surface->w, surface->h};
    SDL_Rect bg_rect = {50, 50, surface->w, surface->h};  // 원래는 {0,0,...}

    SDL_RenderFillRect(r.sdl_renderer, &bg_rect);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(r.sdl_renderer, surface);
    SDL_RenderCopy(r.sdl_renderer, texture, NULL, &bg_rect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    dirty = false;
}


void text::on_click(renderer const&, unsigned int, unsigned int) {}
void text::on_drag(renderer const&, unsigned int, unsigned int, int, int) {}
bool text::needs_update() const { return dirty; }

// button
button::button(widget* w) : content(w) {}

void button::render(renderer const& r) {
    if (content) content->render(r);
}

void button::on_drag(renderer const& r, unsigned int x, unsigned int y, int dx, int dy) {
    if (content) content->on_drag(r, x, y, dx, dy);
}

bool button::needs_update() const {
    return content && content->needs_update();
}

// window
window::window(SDL_Renderer* r, std::string t, widget* c)
    : sdl_renderer(r), title(t), content(c) {}

window::~window() {
    delete content;
}

void window::render(renderer const& r) {
    SDL_SetRenderDrawColor(r.sdl_renderer, 180, 180, 180, 255);
    SDL_Rect frame = {x_offset, y_offset, 300, 200};
    SDL_RenderFillRect(r.sdl_renderer, &frame);

    SDL_SetRenderDrawColor(r.sdl_renderer, 100, 100, 255, 255);
    SDL_Rect titlebar = {x_offset, y_offset, 300, 30};
    SDL_RenderFillRect(r.sdl_renderer, &titlebar);

    if (content) {
        renderer* sub = r.subrenderer(x_offset + 5, y_offset + 35, 290, 160);
        content->render(*sub);
        delete sub;
    }
}


void window::on_click(renderer const& r, unsigned int x, unsigned int y) {
    if (content) content->on_click(r, x, y);
}

void window::on_drag(renderer const& r, unsigned int x, unsigned int y, int dx, int dy) {
    if (content) content->on_drag(r, x, y, dx, dy);
}

bool window::needs_update() const {
    return content && content->needs_update();
}

horizontal::horizontal(widget* l, widget* r) : left(l), right(r) {}
horizontal::~horizontal() { delete left; delete right; }

void horizontal::render(renderer const& r) {
    renderer* left_r = r.subrenderer(0, 0, 400, 800);   // 왼쪽 영역
    renderer* right_r = r.subrenderer(400, 0, 400, 800); // 오른쪽 영역
    left->render(*left_r);
    right->render(*right_r);
    delete left_r;
    delete right_r;
    SDL_RenderSetViewport(r.sdl_renderer, NULL);  // ← 추가

}

void horizontal::on_click(renderer const& r, unsigned int x, unsigned int y) {
    if (x < 200) left->on_click(r, x, y);
    else right->on_click(r, x - 200, y);
}

void horizontal::on_drag(renderer const& r, unsigned int x, unsigned int y, int dx, int dy) {
    if (x < 200) left->on_drag(r, x, y, dx, dy);
    else right->on_drag(r, x - 200, y, dx, dy);
}

bool horizontal::needs_update() const {
    return left->needs_update() || right->needs_update();
}

text_changer_button::text_changer_button(widget* content, text* target)
    : button(content), target_text(target) {}

void text_changer_button::on_click(renderer const&, unsigned int, unsigned int) {
    target_text->set_text("Button clicked!");
}
vertical::vertical(widget* t, widget* b) : top(t), bottom(b) {}
vertical::~vertical() {
    delete top;
    delete bottom;
}

void vertical::render(renderer const& r) {
    renderer* top_r = r.subrenderer(0, 0, 800, 400);   // 위쪽 영역 (화면 높이 800 기준)
    renderer* bottom_r = r.subrenderer(0, 400, 800, 400); // 아래쪽 영역
    top->render(*top_r);
    bottom->render(*bottom_r);
    delete top_r;
    delete bottom_r;

    SDL_RenderSetViewport(r.sdl_renderer, NULL);  // viewport 초기화
}

void vertical::on_click(renderer const& r, unsigned int x, unsigned int y) {
    if (y < 400) top->on_click(r, x, y);
    else bottom->on_click(r, x, y - 400);
}

void vertical::on_drag(renderer const& r, unsigned int x, unsigned int y, int dx, int dy) {
    if (y < 400) top->on_drag(r, x, y, dx, dy);
    else bottom->on_drag(r, x, y - 400, dx, dy);
}

bool vertical::needs_update() const {
    return top->needs_update() || bottom->needs_update();
}

two_windows::two_windows(widget* w1, widget* w2)
    : window1(w1), window2(w2), front_is_first(true) {}

two_windows::~two_windows() {
    delete window1;
    delete window2;
}

void two_windows::render(renderer const& r) {
    // 배경을 검정색으로 지우기
    SDL_SetRenderDrawColor(r.sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(r.sdl_renderer);

    if (front_is_first) {
        window2->render(r);
        window1->render(r);
    } else {
        window1->render(r);
        window2->render(r);
    }
}

void two_windows::on_click(renderer const& r, unsigned int x, unsigned int y) {
    if (front_is_first) {
        window1->on_click(r, x, y);
        front_is_first = false;  // 클릭되었으므로 뒤로 보냄
    } else {
        window2->on_click(r, x, y);
        front_is_first = true;
    }
}

void two_windows::on_drag(renderer const& r, unsigned int x, unsigned int y, int dx, int dy) {
    if (front_is_first) {
        window1->on_drag(r, x, y, dx, dy);
    } else {
        window2->on_drag(r, x, y, dx, dy);
    }
}

bool two_windows::needs_update() const {
    return window1->needs_update() || window2->needs_update();
}

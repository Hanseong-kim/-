#pragma once
#include <string>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

class renderer {
public:
    SDL_Renderer* sdl_renderer;
    renderer(SDL_Renderer* r);
    renderer* subrenderer(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const;
};

class widget {
public:
    virtual void render(renderer const&) = 0;
    virtual void on_click(renderer const&, unsigned int x, unsigned int y) = 0;
    virtual void on_drag(renderer const&, unsigned int x, unsigned int y, int dx, int dy) = 0;
    virtual bool needs_update() const = 0;
    virtual ~widget() {}
};

class checkered : public widget {
public:
    checkered();
    void render(renderer const&) override;
    void on_click(renderer const&, unsigned int x, unsigned int y) override;
    void on_drag(renderer const&, unsigned int x, unsigned int y, int dx, int dy) override;
    bool needs_update() const override;

private:
    bool toggled;
};

class text : public widget {
public:
    text(SDL_Renderer* r, std::string text = "", int font_size = 24,
     float red = 1.0, float green = 1.0, float blue = 1.0,
     float bg_red = 0, float bg_green = 0, float bg_blue = 0);
    void render(renderer const&) override;
    void on_click(renderer const&, unsigned int x, unsigned int y) override;
    void on_drag(renderer const&, unsigned int x, unsigned int y, int dx, int dy) override;
    bool needs_update() const override;
    void set_text(std::string new_text);

private:
    SDL_Renderer* sdl_renderer;
    std::string current_text;
    SDL_Color text_color;
    SDL_Color bg_color;
    bool dirty;
    TTF_Font* font;
};

class button : public widget {
public:
    button(widget* content);
    virtual void on_click(renderer const&, unsigned int x, unsigned int y) = 0;
    void render(renderer const&) override;
    void on_drag(renderer const&, unsigned int x, unsigned int y, int dx, int dy) override;
    bool needs_update() const override;

protected:
    widget* content;
};

class window : public widget {
public:
    window(SDL_Renderer*, std::string title, widget* content);
    void render(renderer const&) override;
    void on_click(renderer const&, unsigned int x, unsigned int y) override;
    void on_drag(renderer const&, unsigned int x, unsigned int y, int dx, int dy) override;
    bool needs_update() const override;
    ~window();

private:
    std::string title;
    widget* content;
    SDL_Renderer* sdl_renderer;
    int x_offset = 0;
    int y_offset = 0;

};

class two_windows : public widget {
public:
    two_windows(widget* w1, widget* w2);
    void render(renderer const&) override;
    void on_click(renderer const&, unsigned int x, unsigned int y) override;
    void on_drag(renderer const&, unsigned int x, unsigned int y, int dx, int dy) override;
    bool needs_update() const override;
    ~two_windows();

private:
    widget* window1;
    widget* window2;
    bool front_is_first;
};

class horizontal : public widget {
public:
    horizontal(widget* left, widget* right);
    void render(renderer const&) override;
    void on_click(renderer const&, unsigned int x, unsigned int y) override;
    void on_drag(renderer const&, unsigned int x, unsigned int y, int dx, int dy) override;
    bool needs_update() const override;
    ~horizontal();

private:
    widget* left;
    widget* right;
};

class vertical : public widget {
public:
    vertical(widget* top, widget* bottom);
    void render(renderer const&) override;
    void on_click(renderer const&, unsigned int x, unsigned int y) override;
    void on_drag(renderer const&, unsigned int x, unsigned int y, int dx, int dy) override;
    bool needs_update() const override;
    ~vertical();

private:
    widget* top;
    widget* bottom;
};

class text_changer_button : public button {
public:
    text_changer_button(widget* content, text* target);
    void on_click(renderer const&, unsigned int x, unsigned int y) override;

private:
    text* target_text;
};

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
class Animation {
public:
    int startFrame;
    int frameCount;
    int frameWidth;
    float frameTime;
    float timer;
    int currentFrame;

    Animation(int start, int count, int width, float timePerFrame):
        startFrame(start), frameCount(count), frameWidth(width),
        frameTime(timePerFrame), timer(0.0f), currentFrame(0){ }

    void update(float deltaTime) {
        timer += deltaTime;
        if (timer >= frameTime) {
            timer = 0.0f;
            currentFrame = (currentFrame + 1) % frameCount;
        }
    }

    SDL_Rect getFrameRect(int y = 0, int frameHeight = 64) const {
        return SDL_Rect{
            (startFrame + currentFrame) * frameWidth,
            y,
            frameWidth,
            frameHeight
        };
    }
};
class Entity {
protected:
    SDL_Rect position;
    SDL_Texture* texture;
    Animation* animation;

public:
    Entity(int x, int y, int w, int h, SDL_Texture* tex, Animation* anim = nullptr)
        : texture(tex), animation(anim) {
        position = { x, y, w, h };
    }
    virtual void update(float deltaTime) {
        if (animation) {
            animation->update(deltaTime);
        }
    }
    virtual void render(SDL_Renderer* renderer) {
        if (texture && animation) {
            SDL_Rect srcRect = animation->getFrameRect();
            SDL_RenderCopy(renderer, texture, &srcRect, &position);
        }
        else if (texture) {
            SDL_RenderCopy(renderer, texture, nullptr, &position);
        }
    }
    void setAnimation(Animation* anim) {
        if (animation != anim) {
            animation = anim;
            animation->currentFrame = 0;
            animation->timer = 0.0f;
        }
    }
    virtual ~Entity() {}
};
bool Init(SDL_Window** window, SDL_Renderer** renderer, int w, int h){
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
    *window = SDL_CreateWindow("Etapa 6", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    return *window && *renderer;
}

SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char* argv[]) {
    //if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) == 0) {
    //    std::cerr << "SDL or SDL_image Init failed: " << SDL_GetError() << std::endl;
    //    return 1;
    //}

    //SDL_Window* window = SDL_CreateWindow("Etapa 6",
    //    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //SDL_Texture* texture = IMG_LoadTexture(renderer, "assets/image.png");
    //if (!texture) {
    //   std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
    //}

    //SDL_Rect destRect = { 300, 200, 200, 200 };
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!Init(&window, &renderer, 800, 600)) return 1;

    //SDL_Texture* texture1 = LoadTexture("assets/image.png", renderer);
    //SDL_Texture* texture2 = LoadTexture("assets/image2.png", renderer);
    //if (!texture1 || !texture2) return 1;

    SDL_Texture* spriteSheet = LoadTexture("assets/spritesheet.png", renderer);
    if (!spriteSheet) return 1;

    //SDL_Rect srcRect = { 0, 0, 64, 64 };
    //int currentFrame = 0;
    //const int totalFrames = 4;
    //const float frameDuration = 0.15f;
    //float frameTimer = 0.0f;
    //Animation walkAnim(0, 4, 64, 0.15f);
    //Animation* walkAnim = new Animation(0, 4, 64, 0.15f);
    Animation* idleAnim = new Animation(0, 1, 64, 0.2f);
    Animation* walkAnim = new Animation(1, 4, 64, 0.15f);
    Entity* player = new Entity(100, 100, 64, 64, spriteSheet, walkAnim);
    Uint32 lastTick = SDL_GetTicks();
    //SDL_Rect dst1 = { 100, 100, 64, 64 };
    //SDL_Rect dst2 = { 300, 200, 64, 64 };

    bool running = true;
    SDL_Event event;
    while (running) {
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }
        Uint32 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        //frameTimer += deltaTime;
        //if (frameTimer >= frameDuration) {
        //    frameTimer = 0.0f;
        //    currentFrame = (currentFrame + 1) % totalFrames;
        //    srcRect.x = currentFrame * srcRect.w;
        //}
        //walkAnim.update(deltaTime);
        //SDL_Rect srcRect = walkAnim.getFrameRect();
        if (keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A]
            || keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D]) {
            player->setAnimation(walkAnim);
        }else {
            player->setAnimation(idleAnim);
        }
        player->update(deltaTime);
        //SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        //if (texture)
        //    SDL_RenderCopy(renderer, texture, NULL, &destRect);

        //SDL_RenderCopy(renderer, texture1, nullptr, &dst1);
        //SDL_RenderCopy(renderer, texture2, nullptr, &dst2);
        //SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dst1);
        player->render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    //SDL_DestroyTexture(texture1);
    //SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
#ifndef ENGINE_GLOBALSTATEMANAGER_HPP
#define ENGINE_GLOBALSTATEMANAGER_HPP

namespace Engine
{
    class GlobalStateManager
    {
    private:
        bool _running = false;

    public:
        static GlobalStateManager* GetInstance()
        {
            static auto* _instance = new GlobalStateManager();
            return _instance;
        }

        bool IsRunning() const
        {
            return _running;
        }

        void Start()
        {
            _running = true;
        }

        void Stop()
        {
            _running = false;
        }

    private:
        GlobalStateManager() = default;
    };
} // namespace Engine

#endif //ENGINE_GLOBALSTATEMANAGER_HPP

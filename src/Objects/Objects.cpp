
#include "Interfaces.hpp"

class Door
    : public IMapObject,
      public IExaminable,
      public IUsable
{
    private:

        std::string _keyId;

        bool _isClosed;

        bool _isLocked;

    public:

        bool
        IsBlocking()
        {
            return _isClosed;
        }

        void
        Examine();

        void
        Use();

};

#ifndef EVENT_RECEIVER_WARPER_HEADER
#define EVENT_RECEIVER_WARPER_HEADER
class EventReceiverWarper : public IEventReceiver{
public:
  // This is the one method that we have to implement
  virtual bool OnEvent(const SEvent& event){
    // Remember whether each key is down or up
    if (event.EventType==irr::EET_KEY_INPUT_EVENT){
      KeyIsDown[event.KeyInput.Key]=event.KeyInput.PressedDown;
      if (event.KeyInput.Key==KEY_KEY_W||event.KeyInput.Key==KEY_KEY_A||event.KeyInput.Key==KEY_KEY_S||event.KeyInput.Key==KEY_KEY_D){
        if ((event.KeyInput.PressedDown) && (!walking)){
          //this will be done once
          walking = true;
          player->animateMove();
        }else if((!event.KeyInput.PressedDown)&&(walking)){
          //this will be done on key up
          walking = false;
          player->animateStand();
        }
      }
    }
    return false;
  }

  // This is used to check whether a key is being held down
  virtual bool IsKeyDown(EKEY_CODE keyCode) const{
    return KeyIsDown[keyCode];
  }

  void setControlPlayer(Player *p){
    this->player=p;
  }

  EventReceiverWarper(){
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
      KeyIsDown[i] = false;
  }

private:
  // Use this array to store the current state of each key
  bool KeyIsDown[KEY_KEY_CODES_COUNT];
  bool walking=false;
  Player *player=nullptr;
}

#endif
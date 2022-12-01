#pragma once


class TimerC {

  private:
        unsigned long _start_time;
        unsigned long _elapsed_time;
  
  public:
      TimerC()
      {
        _start_time = 0;
        _elapsed_time = 0;
      }

      void start()
      {
        if(_start_time != 0)
        {
          return;
        }
        _start_time = millis();
      }
      
      unsigned long getET()
      {
        //in millisecondi
        _elapsed_time = millis() - _start_time;
        return _elapsed_time;
      }
      double getETSec()
      {
        unsigned long et_milli =  getET();
        return et_milli/1000.0;
      }
      void reset()
      {
        _start_time = millis();
        _elapsed_time = 0;
      }
      void stop_()
      {
        if(_start_time == 0)
        {
          return;
        }
        _start_time = 0;
        _elapsed_time = 0;
        
      }
};




class R_TRIG
{
    private:
      bool state;
      bool state_old;  
      bool q;
    public:
      bool getQ()
      {
          return q;
      }
  
      R_TRIG(bool init)
      {
          q = false;
          state = init;
          state_old = init;
      }
      bool call(bool _in)
      {
          state = _in;
          if (state && !state_old)
          {
              //fronte di salita
              q = true;
          }
          else
          {
              q = false;
          }
          state_old = state;
          return q;
      }
};

class Debouncer
{
	private:
		bool buttonState;             // the current reading from the input pin
		bool lastButtonState;   // the previous reading from the input pin
		uint64_t lastDebounceTime = 0;  // the last time the output pin was toggled
		uint64_t debounceDelay;    //[ms] the debounce time; increase if the output flickers
	public:
		Debouncer(bool ButtonStateInit=false, unsigned long delay_ms=50)
		{
			lastButtonState = ButtonStateInit;
			buttonState = ButtonStateInit;
			debounceDelay = delay_ms;
		}
		bool run(int reading)
		{
			// check to see if you just pressed the button
			// (i.e. the input went from LOW to HIGH), and you've waited long enough
			// since the last press to ignore any noise:

			// If the switch changed, due to noise or pressing:
			if (reading != lastButtonState) {
				// reset the debouncing timer
				lastDebounceTime = millis();
			}
			if ((millis() - lastDebounceTime) > debounceDelay) 
			{
				// whatever the reading is at, it's been there for longer than the debounce
				// delay, so take it as the actual current state:

				// if the button state has changed:
				if (reading != buttonState) 
				{
					buttonState = reading;
				}
  			}
			// save the reading. Next time through the loop, it'll be the lastButtonState:
  			lastButtonState = reading;
			return buttonState;
		}
		  
};

class TON 
{
	private:
		TimerC* _timer;
		R_TRIG* _fs, *_fd;
	public:
		bool q;
		uint64_t pt_ms;

	public:
		TON(bool in_=false, uint64_t pt_ms_ = 50)
		{
			pt_ms=pt_ms_;
			_timer = new TimerC();
			_fs = new R_TRIG(in_);
			_fd = new R_TRIG(in_);
			_timer->start();
		}
		bool run(bool in_)
		{
			if ( _fs->call(in_) )
			{
				//sul fronte di salita del ingresso
				_timer->reset();
			}

			if ( _fd->call(in_==false) )
			{
				//sul fronte di discesa del ingresso
				_timer->reset();
			}

			if (in_ && _timer->getET()>=pt_ms)
			{
				q=true;
			}
			else
			{
				q=false;
			}
			return q;
			
		}

		~TON()
		{
			delete _timer;
			delete _fs;
			delete _fd;
		}
};

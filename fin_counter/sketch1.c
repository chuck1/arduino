

int piezo = 1;
int led_pin = 0;
int analog_period = 10;
int led_state = LOW;
int thresh = 500;


int pin_button_reset = 0;


class Button
{
	public:
		Button(int pin):
			_M_pin(pin),
			_M_state(0),
			_M_count(0),
			_M_temp(0)
		{
		}
		void	update()
		{
			int temp = digitalRead(_M_pin);
			if(temp == _M_temp) {
				if(_M_count == 5) return;
				_M_count++;
				if(_M_count == 5) {
					_M_state = _M_temp;
				}
			} else {
				_M_count = 0;
				_M_temp = temp;
			}
		}
	private:
		int _M_state;
		int _M_count;
		int _M_pin;
		int _M_temp;
};

class Sensor
{
	public:
		Sensor():
			_M_pin(pin),
			_M_thresh(thresh)
			_M_period(period)
		{}
		void	update()
		{
			float mean;
			float analog = analogRead(_M_pin);

			_M_sum += analog;
			_M_count++;

			if(_M_count == _M_period) {
				mean = (float)_M_sum / (float)_M_count;

				_M_sum = 0;
				_M_count = 0;

				serial.Print(mean);

				if(mean > _M_thresh) {
					_M_handler(*this);
				}
			}
		}
	private:
		void	(*_M_handler)();
		int	_M_sum;
		int	_M_count;
		int	_M_pin;
		int	_M_thresh;
		int	_M_period;
};

void	toggle_led()
{
	led_state = !led_state;
	digitalWrite(led_pin, led_state);
}

void handler(Sensor & s)
{
	toggle_led();
}

Button button_reset(0, 10);
Sensor sensor(0, 500, 100);

void loop()
{
	button_reset.update();

}





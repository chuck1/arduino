

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
		void	update()
		{
			analog = analogRead(piezo);

			analog_sum += analog;
			analog_count++;

			if(analog_count == analog_period) {
				avg = (float)analog_sum / (float)analog_count;

				analog_sum = 0;
				analog_count = 0;

				serial.Print(avg);

				if(avg > thresh) {
					toggle();
				}
			}
		}
	private:
		void	(*_M_handler)();
		int	_M_sum;
		int	_M_count;
		int	_M_pin;
};

void toggle() {
	led_state = !led_state;
	digitalWrite(led_pin, led_state);
}

Button button_reset(0);

void loop()
{
	button_reset.update();

}





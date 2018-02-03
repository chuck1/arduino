
#define DEBUG (false)

void ditfft2(float * X_R, float * X_I, float * x, int N, int s) {
  if(N==1) {
    // trivial size-1 DFT base case
    //X[0] = x[0];
    X_R[0] = x[0];
    X_I[0] = 0;
  } else {
    // DFT of (x0, x2s, x4s, ...)
    //X0,...,N/2−1 = ditfft2(x, N/2, 2s)
    ditfft2(X_R, X_I, x, N/2, 2*s);
    
    // DFT of (xs, xs+2s, xs+4s, ...)
    //XN/2,...,N−1 = ditfft2(x+s, N/2, 2s)
    ditfft2(X_R+N/2, X_I+N/2, x+s, N/2, 2*s);
    
    // combine DFTs of two halves into full DFT:
    for (int k = 0; k < (N/2-1); ++k) {
      float t_R = X_R[k];
      float t_I = X_I[k];
      
      // 2 pi i k/N
      float b = 2 * M_PI * k/N;
      // exp(−2πi k/N)
      float a_R = cos(b);
      float a_I = -sin(b);
      
      //X[k] = t + exp(-2πi k/N) X[k+N/2]
      //X[k] = t + a * X[k+N/2];
      X_R[k] = t_R + (a_R * X_R[k+N/2] - a_I * X_I[k+N/2]);
      X_I[k] = t_I + (a_R * X_I[k+N/2] + a_I * X_R[k+N/2]);
      
      //X[k+N/2] = t − exp(-2πi k/N) X[k+N/2]
      //X[k+N/2] = t - a * X[k+N/2];
      X_R[k+N/2] = t_R - (a_R * X_R[k+N/2] - a_I * X_I[k+N/2]);
      X_I[k+N/2] = t_I - (a_R * X_I[k+N/2] + a_I * X_R[k+N/2]);
      
    }
  }
}

class Bounce
{
public:
  Bounce(int t, int p0, int p1):
    thresh(t),
    count(0),
    state(0),
    state_temp_last(0),
    state_temp(0),
    duration(0),
    duration_last(0)
  {
    period[0] = p0;
    period[1] = p1;
  }
  void  update()
  {
    duration++;
    
    a = analogRead(0);

    state_temp_last = state_temp;
    
    if(a > thresh) {
      state_temp = 1;
    } else {
      state_temp = 0;
    }


    if(state_temp != state) {

      if(DEBUG) {
      Serial.print("state = ");
      Serial.print(state);
      Serial.print(" state_temp = ");
      Serial.print(state_temp);
      Serial.print(" state_temp_last = ");
      Serial.print(state_temp_last);
      Serial.print(" count = ");
      Serial.print(count);
      Serial.print("\n");
      }
      
      if(count == 0) {
        count++;
      } else {
        if(state_temp == state_temp_last) {
          count++;
        } else {
          count = 0;
        }
      }

      if(count == period[state_temp]) state_change(state_temp);

/*
      if(state_temp == state_temp_last) {

        
        // dont think this will ever happen
        if(count == period[state_temp]) return;

        
        count++;

        Serial.print("period = ");
        Serial.print(period[state_temp]);
        Serial.print("\n");
        
        if(count == period[state_temp]) state_change(state_temp);
        
        
      } else {
        count = 0;
      }
*/
    }

    
  }
  void  state_change(int s)
  {
    state = s;

    count = 0;
    
    duration_last = duration;
    duration = 0;
    
    _M_callback_state_change(*this, s);
  }
  int get_state() { return state; }
private:
  int thresh;
  int count;
  int state;
  int state_temp_last;
  int state_temp;
  int period[2];
  
public:
  int a;
  int duration;
  int duration_last;
  void (*_M_callback_state_change)(Bounce&, int);
};




int a0;
int a;

int thresh = 2;
int count = 0;
int led_pin = 7;
int led_state = LOW;

char buf[64];

Bounce piezo(1, 1000, 1);

void on_state_change(Bounce & b, int s)
{
  if(s == 1) {
    count++;
    led_state = !led_state;
    digitalWrite(led_pin, led_state);
  }
  
  sprintf(buf, "%6i%6i%6i\n", s, count, piezo.duration_last);
  Serial.print(buf);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
  Serial.print("hello\n");

  piezo._M_callback_state_change = &on_state_change;
}

void loop() {
  // put your main code here, to run repeatedly:

  if(DEBUG) {
    a0 = a;
    a = analogRead(0);
    if (a > 0) {
      Serial.print(a);
      Serial.print("\n");
    } else {
      
    }
  }
  
  piezo.update();

}



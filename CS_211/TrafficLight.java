
public class TrafficLight 
{
	public static final int GREEN = 1;
	public static final int RED = 2;
	
	private int lightState;
	private int lightDuration;
	private int lightCounter;

	/***
	 * Constructor - initializes TrafficLight starting state
	 * @param initialState
	 * Red or Green
	 * @param duration
	 * Length of light cycle (number of seconds before the state toggles)
	 */
	public TrafficLight(int initialState, int duration)
	{
		lightState = initialState;
		lightDuration = duration;
		lightCounter = duration;
	}

	/***
	 * Gets the current state of the light
	 * @return
	 * Current state of the light
	 */
	public int getState()
	{
		return lightState;
	}
	
	/***
	 * Decrements the number of remaining seconds in the current cycle.
	 * When the remaining seconds goes to zero, the cycle state switches
	 * and the remaining seconds are reset.
	 */
	public void decrementRemainingSeconds()
	{
		if (lightCounter > 0)
		{
			lightCounter--;
		}
		else
		{
            lightCounter = lightDuration;
            lightState = ((lightState == RED) ? GREEN : RED);
		}
	}	
}

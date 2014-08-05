// FILE: Traffic.java
// The simlights method was written by: Ethan Crawford
// A simple program to simulate two synchronized traffic lights. The
// program prints a table indicating the average wait-time for cars coming
// through the two lights.

// The following classes are available in the directory
// http://www.cs.colorado.edu/~main/edu/colorado/simulations, with 
// documentation in http://www.cs.colorado.edu/~main/docs/.
// The three .java files must be present on your machine in
// a subdirectory called edu/colorado/simulations, and the edu
// must itself be in a subdirectory that is part of your Java
// CLASSPATH.

// The following class is available in the directory
// http://www.cs.colorado.edu/~main/edu/colorado/collections, with 
// documentation in http://www.cs.colorado.edu/~main/docs/.
// The .java files must be present on your machine in
// a subdirectory called edu/colorado/collections, and the edu
// must itself be in a subdirectory that is part of your Java
// CLASSPATH.
public class Traffic
{
    public static void main(String[] args)
    {
        final double ARRIVAL_PROB = 0.1; // Prob. of car arrival in a second.
        final int TRAVEL_TIME = 40;     // Seconds to travel between lights.
        final int TOTAL_TIME = 6000;    // Total seconds in the simulation.
        final int SPAN = 30;            // How long is a light red or green?

        int offset;

        for (offset = 0; offset < 2 * SPAN; offset++)
        {
            System.out.printf("Offset of %d seconds results in average wait of %f seconds.\r\n", offset, 
            		simlights(offset, SPAN, ARRIVAL_PROB, TRAVEL_TIME, TOTAL_TIME)
            );
        }
    }
    
    /***
		Simulates traffic flowing through two synchronized traffic lights.
        Cars arrive at the first light, wait for green, move to the second 
        light, wait for green, and exit.
        On green, only one car can move through the the light per second. 
        Uses two Averagers, one for the average wait time at each light.
        Uses two Queues, one for the cars at each light. Each Averager 
        contains the arrival times of cars that are waiting to go through 
        the light. 
        Uses one BooleanSource to determine when new cars arrive at the 
        first light. 
        Uses two TrafficLight objects.
        
     * @param offset
    	The number of seconds between the time that the first light changes to 
    	green and the time that the second light changes to green. If offset 
    	is zero, the two lights change to green at the same time. If offset is 
    	10, the first light changes to green 10 seconds ahead of the second.
     * @param span
    	The length of time that each light stays red or green, in seconds.
     * @param arrivalProb
    	The probability that a car arrives at the first traffic light in 
    	any given second.
     * @param travelTime
 		The number of seconds that it takes a car to travel from the first 
 		traffic light to the second.
      * @param totalTime
		Total number of seconds to run the simulation.
     * @return
		The sum of the two averages. If no cars make it through the second light, -1.
     */
    public static double simlights(int offset, int span, double arrivalProb, int travelTime, int totalTime)
    {
    	double ret = -1.0;
        int vehicleInTransit = 0;
        Averager firstLightAverage = new Averager();
        Averager secondLightAverage = new Averager();
        TrafficLight firstLight = new TrafficLight(TrafficLight.RED, span);
        TrafficLight secondLight = new TrafficLight(TrafficLight.RED, span);
        IntQueue firstLightQueue = new IntQueue();
        IntQueue secondLightQueue = new IntQueue();
        BooleanSource newCarArrivalGenerator = new BooleanSource(arrivalProb);

        while (totalTime-- > 0)
        {
        	// If a new car arrives at the first light, save the simulation time at which
            // it arrived.
            if (newCarArrivalGenerator.query())
            {
                firstLightQueue.insert(totalTime);
            }

            firstLight.decrementRemainingSeconds();

            // offset is the initial delay between the two lights. The first light starts running
            // immediately; the second light runs behind the first 'offset' seconds. This offset
            // will persist over the life of the simulation, since the light durations are the same.
            if (offset > 0)
            {
                offset--;
            }
            else
            {
            	secondLight.decrementRemainingSeconds();
            }
           
            // Pull a car off the queue if one is present and the light is green.
            // Calculate and store the number of passed seconds since the car entered the queue
            if (firstLight.getState() == TrafficLight.GREEN && !firstLightQueue.isEmpty())
            {
                firstLightAverage.addNumber((double)(firstLightQueue.getFront() - totalTime));
                secondLightQueue.insert(totalTime);
            }

            // Account for the vehicle travel time between the first and second lights by pulling 
            // the lead vehicle off the queue and checking if it has "arrived" at the second light 
            // (if enough time has passed).
            if (vehicleInTransit == 0 && (!secondLightQueue.isEmpty()))
            {
            	vehicleInTransit = secondLightQueue.getFront();
            }

            // If enough time has passed and the light is green, pass the vehicle through the second light.
            if (vehicleInTransit != 0 && 
            		(totalTime + travelTime <= vehicleInTransit) && (secondLight.getState() == TrafficLight.GREEN))
            {
            	// When calculating the average, subtract out the travel time. 
            	// (We are only interested in actual queue wait times.)
                secondLightAverage.addNumber((double)(vehicleInTransit - (totalTime + travelTime)));
                vehicleInTransit = 0;
            }
        }

        // If at least one vehicle has passed through the queue, sum the averages and return that value.
        if (secondLightAverage.howManyNumbers() > 0)
        {
            ret = firstLightAverage.average() + secondLightAverage.average();   
        }

       return ret; 
    }
}

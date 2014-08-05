import java.util.logging.ConsoleHandler;

	/* Class to drive the pseudorandom class and generate
	 * tables of data.
	*/
public class Driver
{
	/*
	/***
	 * Test the performance of a collection class
	 * @param n
	 * The number of iterations to run
	 */
	public static void testBagPerformance(int n)
	{
        double elapsed = 0;     // elapsed time
        long start;         // start time
        
        // Test the desired collection class
//        IntTreeBag bag = new IntTreeBag();
//        IntLinkedBag bag = new IntLinkedBag();
        IntArrayBag bag = new IntArrayBag();
        
    	System.out.println("Item Count (n)\tElapsed Time");
        for (int i = 1000; i <= n; i += 20000)
        {
        	bag.clear();
        	
        	start = (int)System.currentTimeMillis();
        	
    		for (int j = 0; j < i; j++)
    		{
    			bag.add((int)(1 + j * Math.random()));
    		}
        	
        	for (int k = 1; k < i; k++)
        	{
        		bag.countOccurrences(k);
        	}
        	
        	elapsed = (int)System.currentTimeMillis() - start;
        	
        	System.out.printf("%d\t%f\n", bag.size(), elapsed);
        }
	}
	
    public static void main(String[] args)
    {
    	/*
//    	testBagPerformance(610000);
    	IntTreeBag bag = new IntTreeBag();
    	
    	for (int i = 0; i < 100; i++)
    	{
    		bag.add((int)(1000.*(Math.random())));
    	}
    	
    	bag.printTree();
    	*/

    	double zero;
    	try
    	{
	    	Polynomial foo  = new Polynomial();
	    	foo.addTerm(1, 1);
	    	foo.addTerm(5, 2);
	    	foo.addTerm(-3, 3);
	    	zero = Polynomial.findZero(foo, 5, -5, 0.000000001);
	    	System.out.println(zero);
    	}
    	// Only InvalidParameterException and any exception
    	// that inherits from it will be caught
    	catch (java.security.InvalidParameterException e)
    	{
    		
    	}
    	// Anything that inherits from Exception (all other
    	// exceptions) will be caught
    	catch (Exception e)
    	{
    		
    	}
    	
    	Integer[] unsorted = new Integer[] 
        {40, 20, 10, 80, 60, 50, 7, 30, 100, 90, 70, 38, 38,};
    	
    	for (Integer i : unsorted)
    	{
    		System.out.print(i + " ");
    	}
		System.out.println();
		
    	GenericQuickSort.quickSort(unsorted, 0, unsorted.length - 1);
    	for (Integer i : unsorted)
    	{
    		System.out.print(i + " ");
    	}
		System.out.println();
		
    }
}

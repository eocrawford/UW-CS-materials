import java.util.NoSuchElementException;

public class GenericTester<T>
{
	/***
	 * 1. Chapter 5, Exercise #11, page 252, occurrences.  
	 * In this context, the method is static, and an array 
	 * is passed in as an argument
	 * 
	 * Iterates an array of objects of type 'T', looking for a target object.
	 * @param <T> 
	 * Object data type
	 * @param data
	 * Array of objects
	 * @param target
	 * Target object for which to search
	 * @return
	 * Count of objects in 'data' equal to 'target'
	 */
	public static <T> int occurrences(T[] data, T target)
	{
		int ret = 0;
		
		for (T item : data)
		{
			if (item != null && item.equals(target))
			{
				ret++;
			}
		}
		
		return ret;
	}


	/***
	 * 2. Chapter 5, Exercise #26, page 285, comparable test.  
	 * Write a complete method that can be inserted into the 
	 * ArrayBag generic implementation, to test for comparable data.
	 * 
	 * Examines the provided Object to determine if it implements the Comparable interface.
	 * @param <T> 
	 * Object data type
	 * @param x 
	 * The Object in question
	 * @return
	 * True if the object implements Comparable, false otherwise
	 */
	public static <T> boolean implementsStringComparable(T x)
	{
		// Java does not appear to support instanceof with a generic type.
		// Attempting to use Comparable<String> results in the following
		// compilation error:
		// "Cannot perform instanceof check against parameterized type 
		// Comparable<String>. Use instead its raw form Comparable since generic 
		// type information will be erased at runtime".

		return (x instanceof Comparable);
	}

	/***
	 * 3. Chapter 6, Project #2, page 345, stack itemAt.  
	 * Write the complete method.
	 * 
	 * Retrieves a Stack object at index n
	 * @param n 
	 * The stack index of the desired Object
	 * @return
	 * The item that is n from the top (n == 0 is the top)
	 */
    public Object itemAt(int n) throws EmptyStackException, NoSuchElementException
    {
        Object ret = this.top;

        if (n < 0 || n > this.size())
        {
            throw new NoSuchElementException("itemAt(): Invalid value for n");
        }

        if (ret == null)
        {
            throw new EmptyStackException("itemAt(): Empty stack");
        }

        // Walk the linked list to the desired point in the stack.
        for (int i = 0; i < n; i++)
        {
            ret = ret.getLink();
        }

        return ret;
    }

	/***
	 * 4. Chapter 7, Exercise #17, page 388, nextIndex.
	 * Implement the nextIndex method for the array version of the queue.
	 * 
	 * Gets the next available index in a queue. This method is used to 
	 * "wrap" the index of an array to reuse the elements of the array 
	 * as a queue container. 
	 * @param currentIndex 
	 * The index to use to calculate the next index.
	 * @return
	 * currentIndex + 1 if that value is less than data.length; 0 otherwise.
	 */
    private int nextIndex(int currentIndex)
    {
        if (currentIndex < 0 || currentIndex > data.length)
        {
            throw new NoSuchElementException("nextIndex(): Invalid value for currentIndex");
        }
        
        return (++currentIndex >= data.length ? 0 : currentIndex);
    }
}

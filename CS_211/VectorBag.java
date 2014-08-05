import java.util.Vector;

/***
 * Extends Vector<E>, adding in the missing Michael Main methods
 * implemented in the LinkedBag<E> class on page 772.
 * @author ethancr
 *
 * @param <E>
 */
public class VectorBag<E> extends Vector<E>
{
	/***
	 * Count the occurrences of item
	 * @param item
	 * @return
	 * number of occurrences
	 */
	public int countOccurrences(E item)
	{
		int count = 0;
		
		for (int i = 0; i < super.size(); i++)
		{
			if (item.equals(super.get(i)))
			{
				count++;
			}
		}
		
		return count;
	}

	/***
	 * Get a random item from the list
	 * @return
	 * random item
	 */
	public E grab()
	{
		return super.get(((int)Math.random() * super.size()));
	}
	
	/***
	 * Add two bags to a new VectorBag
	 * @param <E>
	 * @param one
	 * @param two
	 * @return
	 * new VectorBag
	 */
	public static <E> VectorBag<E> union(VectorBag<E> one, VectorBag<E> two)
	{
		VectorBag<E> ret = new VectorBag<E>();
		ret.addAll(one);
		ret.addAll(two);
		return ret;
	}
}

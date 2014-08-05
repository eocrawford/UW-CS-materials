
public class GenericQuickSort
{
	/***
	 * Sort an array of Comparable elements
	 * @param array
	 * array
	 * @param left
	 * starting offset
	 * @param right
	 * count of items after offset + left
	 */
	public static void quickSort(Comparable[] array, int left, int right)
	{
		if (right <= left || array.length <= 1)
		{
			return;
		}
		
		int split = partition(array, left, right);
		quickSort(array, left, split - 1);
		quickSort(array, split + 1, right);
	}
	
	/**
	 * Sort the elements from left to right in array
	 * @param array
	 * array
	 * @param left
	 * @param right
	 * @return
	 * pivot index
	 */
	private static int partition(Comparable[] array, int left, int right)
	{
		int pivotIndex = left;
		Comparable pivot = array[pivotIndex];
		
		while (true)
		{
			// The pivot is in the left position, so advance the starting
			// index before comparing the next item.
			while (++left < right && array[left].compareTo(pivot) < 0);

			// While the index hasn't crossed and the elements are sorted,
			// advance the right position 
			while (left <= right && array[right].compareTo(pivot) > 0)
			{
				right--;
			}
			
			// Indices are crossed. Move the pivot to its
			// final resting place and exit the loop.
			if (left > right)
			{
				swap(array, pivotIndex, right);
				break;
			}
			
			swap(array, right, left);
		}

		return right;
	}

	/***
	 * Helper code to swap two elements in an array
	 * @param array
	 * @param x
	 * @param y
	 */
	private static void swap(Comparable[] array, int x, int y)
	{
		// Do not swap identical elements
		if (x == y)
		{
			return;
		}
		
		Comparable temp = array[x];
		array[x] = array[y];
		array[y] = temp;
	}
}

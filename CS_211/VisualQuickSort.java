import java.applet.*;
import java.awt.*;
import java.awt.event.*;

/***
 * Plays a simple animation of the QuickSort algorithm using a built-in
 * array of integers.
 * @author 
 * Ethan Crawford
 */
public class VisualQuickSort extends Applet implements ActionListener
{
	private Button goButton = new Button("Do Sort");
	
	private Integer[] vals 
	    = new Integer[]{40,20,10,80,60,50,7,30,100,90,70,38,38};
	
	private TextField[] numbers = new TextField[vals.length];
	
	private Integer[] unsorted;	

	/***
	 * Lay out the controls
	 */
	public void init()
	{
		// Use a simple flow layout
		setLayout(new FlowLayout(FlowLayout.LEFT));
		
		
		add(goButton);
		goButton.addActionListener(this);
		
		for (int i = 0; i < numbers.length; i++)
		{
			numbers[i] = new TextField(vals[i].toString());
			add(numbers[i]);
		}
	}

	/***
	 * Handle control actions
	 */
	public void actionPerformed(ActionEvent e) 
	{
		if (e.getSource() == goButton)
		{
			// disable the button for the duration of the sort
			goButton.setEnabled(false);
			
			// Copy the unsorted array for sorting purposes
			unsorted = vals.clone();
			
			// Sort it
			quickSort(unsorted, 0, unsorted.length - 1);
			
			// Re-enable the go button
			goButton.setEnabled(true);
		}
	}

	/***
	 * Animate the TextFields to show the process of a quicksort
	 * @param myX
	 * First param being swapped
	 * @param myY
	 * Second param being swapped
	 */
	private void highlightValues(int myX, int myY)
	{
		for (int i = 0; i < numbers.length; i++)
		{
			if (i == myX)
			{
				numbers[i].setForeground(new Color(255, 0, 0));
			}
			else if (i == myY)
			{
				numbers[i].setForeground(new Color(0, 0, 255));
			}
			else
			{
				numbers[i].setForeground(new Color(0, 0, 0));
			}
			
			numbers[i].setText(unsorted[i].toString());
		}
		
		try{Thread.sleep(850);}catch(InterruptedException e){}
	}
	
	/***
	 * Quicksort implementation, recursive
	 * @param array
	 * array
	 * @param left
	 * left element (offset)
	 * @param right
	 * right element (offset + length)
	 */
	private void quickSort(Comparable[] array, int left, int right)
	{
		if (right <= left || array.length <= 1)
		{
			return;
		}
		
		int split = partition(array, left, right);
		quickSort(array, left, split - 1);
		quickSort(array, split + 1, right);
	}

	/***
	 * Partition for quicksort
	 * @param array
	 * array
	 * @param left
	 * left element (offset)
	 * @param right
	 * right element (offset + length)
	 * @return
	 * partition index
	 */
	private int partition(Comparable[] array, int left, int right)
	{
		int pivotIndex = left;
		Comparable pivot = array[pivotIndex];
		
		while (true)
		{
			// The pivot is in the left position, so advance the starting
			// index before comparing the next item.
			while (++left < right && array[left].compareTo(pivot) < 0);
			
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
	 * Swap elements x and y in the array
	 * @param array
s	 * @param x
	 * @param y
	 */
	private void swap(Comparable[] array, int x, int y)
	{
		if (x == y)
		{
			return;
		}
		
		highlightValues(x, y);
		
		Comparable temp = array[x];
		array[x] = array[y];
		array[y] = temp;
		
		highlightValues(y, x);
	}
}

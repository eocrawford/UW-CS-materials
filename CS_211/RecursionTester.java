import javax.swing.tree.DefaultMutableTreeNode;
import java.util.*;

public class RecursionTester 
{
	public static void demonstrateTOH(int n)
	{
		IntStack leftTower = new IntStack();
		IntStack middleTower = new IntStack();
		IntStack rightTower = new IntStack();

		// Build the tower of rings, largest on the bottom
		for (int i = n; i > 0; i--)
		{
			leftTower.push(i);
		}

		// Call the recursive helper, passing in the three towers and the number of rings
		towersOfHanoi(n, leftTower, middleTower, rightTower);
	}
	
	/***
	 * Implements the "Towers of Hanoi" mathematical puzzle, solved recursively.
	 * 
	 * @param ringCount
	 * The number of "rings" to move from tower 1 to tower 3
	 * @param t1
	 * Tower one - the starting tower containing the initial ring stack
	 * @param t2
	 * Tower two - intermediate tower for use in transfering rings
	 * @param t3
	 * Tower three - the final tower onto which the ring stack must be moved
	 */
	public static void towersOfHanoi(
		int ringCount,
		IntStack t1, 
		IntStack t2,
		IntStack t3)
	{
		if (ringCount <= 0)
		{
			return;
		}
		
		towersOfHanoi(ringCount - 1, t1, t3, t2);
		System.out.printf("%s ", t1.peek());
		t2.push(t1.pop());
		towersOfHanoi(ringCount - 1, t3, t2, t1);
	}
	
	public static void HashtableTest()
	{
		Hashtable<String, String> phonebook = new Hashtable<String, String>(811);
		phonebook.put("Jean Luc", "622-1701");
		phonebook.put("Katherine", "627-4656");
		phonebook.put("Benjamin", "622-1764");
		System.out.println(phonebook.get("Katherine"));
		
		Enumeration<String> e = phonebook.keys();
		
		while (e.hasMoreElements())
		{
			System.out.println(phonebook.get(e.nextElement()));
		}
	}
	
	public static void main(String[] args) 
	{
	}
}

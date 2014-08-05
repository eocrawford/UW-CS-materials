public class IntTreeBag
{
   private IntBTNode root;
   
   public void clear()
   {
	   root = null;
   }

   /**
   * Insert a new element into this bag.
   * @param <CODE>element</CODE>
   *   the new element that is being inserted
   * <dt><b>Postcondition:</b><dd>
   *   A new copy of the element has been added to this bag.
   * @exception OutOfMemoryError
   *   Indicates insufficient memory a new IntBTNode.
   **/
   public void add(int element)
   {
	   if (root == null)
	   {
		   root = new IntBTNode(element, null, null);
	   }
	   else
	   {
		   addRecursive(element, root);
	   }
   }
   
   private void addRecursive(int element, IntBTNode current)
   {
	   if (element > current.getData())
	   {
		   if (current.getRight() == null)
		   {
			   current.setRight(new IntBTNode(element, null, null));
		   }
		   else
		   {
			   addRecursive(element, current.getRight());
		   }
	   }
	   else
	   {
		   if (current.getLeft() == null)
		   {
			   current.setLeft(new IntBTNode(element, null, null));
		   }
		   else
		   {
			   addRecursive(element, current.getLeft());
		   }
	   }
   }
   
   /**
   * Add the contents of another bag to this bag.
   * @param <CODE>addend</CODE>
   *   a bag whose contents will be added to this bag
   * <dt><b>Precondition:</b><dd>
   *   The parameter, <CODE>addend</CODE>, is not null.
   * <dt><b>Postcondition:</b><dd>
   *   The elements from <CODE>addend</CODE> have been added to this bag.
   * @exception IllegalArgumentException
   *   Indicates that <CODE>addend</CODE> is null.
   * @exception OutOfMemoryError
   *   Indicates insufficient memory to increase the size of the bag.
   **/
   public void addAll(IntTreeBag addend)
   {
	   addAllRecursive(addend.root);
   }
   
   private void addAllRecursive(IntBTNode current)
   {
	   if (current != null)
	   {
		   add(current.getData());
		   addAllRecursive(current.getRight());
		   addAllRecursive(current.getLeft());
	   }
   }
   
   /**
   * Accessor method to count the number of occurrences of a particular element
   * in this bag.
   * @param <CODE>target</CODE>
   *   the element that needs to be counted
   * @return
   *   the number of times that <CODE>target</CODE> occurs in this bag
   **/
   public int countOccurrences(int target)
   {
	   return countOccurrencesRecursive(target, root);
   }
   
   private int countOccurrencesRecursive(int target, IntBTNode current)
   {
	   int count = 0;
	   
	   if (current != null)
	   {
		   if (target > current.getData())
		   {
			   count += countOccurrencesRecursive(target, current.getRight());
		   }
		   else
		   {
			   if (target == current.getData())
			   {
				   count++;
			   }
			   
			   count += countOccurrencesRecursive(target, current.getLeft());
		   }
	   }
	   
	   return count;
   }
   
   public int depth()
   {
	   return depthRecursive(root, 0);
   }
   
   private int depthRecursive(IntBTNode current, int currentDepth)
   {
	   int depth = currentDepth;
	   
	   if (current != null)
	   {
		   depth = java.lang.Math.max(
				   depthRecursive(current.getRight(), currentDepth + 1),  
				   depthRecursive(current.getLeft(), currentDepth + 1));
	   }
	   
	   return depth;
   }
   
   public void printTree()
   {
	   printTreeRecursive(root, 0);
   }
   
   private void printTreeRecursive(IntBTNode current, int depth)
   {
	  if (current != null)
	  {
		  printTreeRecursive(current.getRight(), depth + 1);
	
		  final char columnSeparator = ':';
		  int spacer = 3;
		  int i;
		  StringBuffer str = new StringBuffer();
		  StringBuffer val = new StringBuffer();
		  
		  val.append(current.getData());

		  // Generate separator columns
		  for (i = 0; i < depth; i++)
		  {
			  // Pad the columns with whitespace
			  for (int j = 0; j < spacer; j++)
			  {
				  System.out.print(" ");
			  }

			  // Generate the column separator
			  System.out.print(columnSeparator);
		  }

		  // Generate the pointing tail for the tree value
		  for (i = 0; i < spacer - 1; i++)
		  {
			  str.append(" ");
		  }
		  
		  str.append("->");

		  // Tack on the value to the pointing tail
		  str.append(val);
	 
		  i = val.length();
	
		  // Pad with whitespace to align the next column
		  if( i > 0 && spacer > i)
		  {
			  str.append(' ');
			  i++;
		  }

		  // Write the column separator
		  str.append(columnSeparator);
		  System.out.println(str);
	     
		  printTreeRecursive(current.getLeft(), depth + 1);
	  }
   }
   
   /**
   * Determine the number of elements in this bag.
   * @param - none
   * @return
   *   the number of elements in this bag
   **/
   public int size()
   {
      return IntBTNode.treeSize(root);
   }
}
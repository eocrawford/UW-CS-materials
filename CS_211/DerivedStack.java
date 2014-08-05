
import java.util.EmptyStackException;

/******************************************************************************
* A <CODE>DerivedStack</CODE> is a <CODE>LinkedSeq</CODE> that can easily
* be used as if it were a stack of Objects. The stack's objects are kept in
* the sequence of the superclass, with the top of the stack at the head of
* the sequence.
*
* <dt><b>Java Source Code for this class:</b><dd>
*   Not available because this is a Programming Project for my students.
*
* @author Michael Main 
*   <A HREF="mailto:main@colorado.edu"> (main@colorado.edu) </A>
*
* @version
*   Jun 12, 1998
*
* @see LinkedSeq
* @see ObjectStack
* @see ObjectLinkedStack
******************************************************************************/
public class DerivedStack extends LinkedSeq
{
   // Invariant of the DerivedStack class:
   //  The Stack's Objects are kept in the sequence of the superclass, with the
   //  top of the stack at the head of the sequence.  

   /**
   * Determine whether this stack is empty.
   * @param - none
   * @return
   *   <CODE>true</CODE> if this stack is empty;
   *   <CODE>false</CODE> otherwise. 
   **/
   public boolean isEmpty( )
   {
      return (size( ) == 0);
   }
   
   
   /**
   * Get the top item of this stack, without removing the item.
   * @param - none
   * <dt><b>Precondition:</b><dd>
   *   This stack is not empty.
   * @return
   *   the top item of the stack
   * @exception EmptyStackException
   *   Indicates that this stack is empty.
   **/  
   public Object peek( )
   {
      if (size( ) < 0)
         throw new EmptyStackException( );
      start( );
      return getCurrent( );
   }  


   /**
   * Get the top item, removing it from this stack.
   * @param - none
   * <dt><b>Precondition:</b><dd>
   *   This stack is not empty.
   * <dt><b>Postcondition:</b><dd>
   *   The return value is the top item of this stack, and the item has
   *   been removed.
   * @exception EmptyStackException
   *   Indicates that this stack is empty.
   **/                      
   public Object pop( )
   {
      Object answer;
      
      if (size( ) < 0)
         throw new EmptyStackException( );
      start( );
      answer = getCurrent( );
      removeCurrent( );
      return answer;
   }
   
   
   /**
   * Push a new item onto this stack. The new item may be the null
   * reference.
   * @param <CODE>item</CODE>
   *   the item to be pushed onto this stack 
   * <dt><b>Postcondition:</b><dd>
   *   The item has been pushed onto this stack.
   * @exception OutOfMemoryError
   *   Indicates insufficient memory for increasing the stack's capacity.
   **/ 
   public void push(Object element)
   {
      start( );
      addBefore(element);
   }
      
}
           

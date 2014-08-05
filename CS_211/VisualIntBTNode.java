
public class VisualIntBTNode extends IntBTNode
{
	public int offset;
	
	public VisualIntBTNode(IntBTNode node)
	{
		super(node.data, node.left, node.right);
	}
}

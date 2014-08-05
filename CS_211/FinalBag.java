
public class FinalBag extends IntArrayBag
{
	public void add(int element)
	{
		if (countOccurrences(element) <= 0)
		{
			super.add(element);
		}
	}
	
	public void addMany(int... elements)
	{
		for (int i : elements)
		{
			this.add(i);
		}
	}
	
	public void addAll(IntArrayBag addend)
	{
		for (int i = 0; i < addend.size(); i++)
		{
			this.add(addend.getItem[i]);
		}
	}
}

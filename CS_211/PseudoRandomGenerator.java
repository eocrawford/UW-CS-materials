
public class PseudoRandomGenerator 
{
	/* Create a new random number generator with the provided parameters
	 * @param multiplier  
	 * @param seed
	 * @param increment
	 * @param modulus
	*/
	public PseudoRandomGenerator(long multiplier, long seed, long increment, long modulus)
	{
		this.multiplier = multiplier;
		this.seed = seed;
		this.increment = increment;
		this.modulus = modulus;
	}
	
	public void foo(PseudoRandomGenerator one, PseudoRandomGenerator two, PseudoRandomGenerator three)
	{
		one.setSeed(69);
		two = new PseudoRandomGenerator(1, 1, 1, 1);
		three = new PseudoRandomGenerator(1, 1, 1, 1);
	}
	
	/* Changes the seed
	 * @param seed
	*/
	public void setSeed(long seed)
	{
		this.seed = seed;
	}
	
	/* Gets the current seed value
	 * @return seed
	*/
	public long getSeed()
	{
		return this.seed;
	}
	
	/* Generates a pseudo-random number
	 * @return The next generated random number
	*/
	public long nextLong()
	{
		this.seed = ((this.multiplier * this.seed + this.increment) % this.modulus);
		return this.seed;
	}
	
	/* Returns a pseudorandom floating point value within the range [0.0, 1.0)
	 * @return Pseudorandom floating point value
	*/
	public float nextFloat()
	{
		return ((float)this.nextLong() / (float)this.modulus);
	}
	
	/* Generates a pseudo-random number with a Gaussian distribution
	 * @param median
	 * @param variance
	 * @return The next generated random number with a Gaussian distribution
	*/
	public double nextGaussian(long median, long variance)
	{
		int operandCount = 3;
		double operands = 0;
		
		for (int i = 0; i < operandCount; i++)
		{
			operands += this.nextFloat();
		}
		
		return (median + (double)(2 * operands - 3) * variance);
	}
	
	private long multiplier;
	private long seed;
	private long increment;
	private long modulus;
}

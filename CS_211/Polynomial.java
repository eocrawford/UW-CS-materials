import java.security.InvalidParameterException;
import java.util.*;

public class Polynomial extends ArrayList
{
	/***
	 * Add a term to the polynomial
	 * @param coefficient
	 * The coefficient of the term.
	 * @param exponent
	 * The exponent value of the term
	 */
	public void addTerm(double coefficient, int exponent)
	{
		// Ensure that sufficient internal elements exist
		while (exponent >= super.size())
		{
			super.add(new Double(0));
		}

		// Add the polynomial at the desired index. The index
		// will serve as exponent, later.
		super.add(exponent, coefficient);
	}

	/***
	 * Evaluate f(x) for the polynomial
	 * @param x
	 * Parameter to evaluate
	 * @return
	 * Result of f(x)
	 */
	public double f(double x)
	{
		double ret = 0;
		
		for (int i = 0; i < super.size(); i++)
		{
			ret += java.lang.Math.pow(((Double)super.get(i)) * x, (double)i);
		}
		
		return ret;
	}

	/***
	 * Given a starting point and an ending point, find a zero for a 
	 * continuous polynomial within a variance of epsilon.
	 * @param f
	 * Polynomial
	 * @param x
	 * Left endpoint
	 * @param z
	 * right endpoint
	 * @param epsilon
	 * Variance
	 * @return
	 * The zero, within epsilon.
	 */
	public static double findZero(
			Polynomial f, 
			double x, 
			double z, 
			double epsilon)
	{
		double fx, fz, ret = 0;
		
		if (epsilon <= 0)
		{
			throw new InvalidParameterException("Epsilon must be greater than 0");
		}
		
		fx = f.f(x);
		fz = f.f(z);
		
		if (fx > 0 || fz < 0)
		{
			throw new InvalidParameterException("f(x) must be <= 0 and f(z) must be >= 0");
		}
		
		// Implement the bisection root-finding algorithm
		// This is a binary search.
		while (x - z > epsilon)
		{
			ret = (z + x) / 2;
			
			if (f.f(ret) * f.f(z) > 0)
			{
				z = ret;
			}
			else
			{
				x = ret;
			}
		}
		
		return ret;
	}

}

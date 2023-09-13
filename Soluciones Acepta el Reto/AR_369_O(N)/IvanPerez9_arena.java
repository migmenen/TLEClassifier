/**
 * 
 */
import java.util.Scanner;

/**
 * @author ivan
 *
 *  https://github.com/IvanPerez9
 *  
 *  Acepta el Reto 369 - Contando en la arena
 */
public class IvanPerez9_arena{

	public static void main(String[] args) {
		
		Scanner entrada = new Scanner(System.in);
		int numLineas = entrada.nextInt();
		
		while (numLineas != 0) {
			for (int i = 0; i < numLineas; i++) {
				System.out.print("1");
			}
			System.out.println();
			numLineas = entrada.nextInt();
		}
		entrada.close();
		
	}
	
}
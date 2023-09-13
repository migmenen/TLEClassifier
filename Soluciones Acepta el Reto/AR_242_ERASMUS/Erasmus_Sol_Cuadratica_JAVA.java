import java.util.Scanner;

public class Erasmus_Sol_Cuadratica_JAVA{
    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        long numCasos;
        long[] arrayCasos;

        numCasos = sc.nextLong();
        while (numCasos != 0) {
            arrayCasos = new long[(int) numCasos];
            for (int i = 0; i < numCasos; ++i) {
                arrayCasos[i] = sc.nextLong();
            }
            resuelveCaso(numCasos, arrayCasos);
            numCasos = sc.nextLong();
        }

    }

    static void resuelveCaso(long a, long[] b) {
        long cont = 0, aux = 0;

        for (int i = 0; i < a; i++) 
        {
            aux = 0;
            for(int k=i; k<a; k++)
            {
                aux += b[k];
            }
            // multiplico por la suma del total restante
            cont += aux * b[i];
        }
        System.out.println(cont);
    }
}
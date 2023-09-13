import java.util.Scanner;

public class Erasmus_Sol_Lineal_JAVA{
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        long numCasos, total = 0;
        long[] arrayCasos;

        numCasos = sc.nextLong();
        while (numCasos != 0) {
            arrayCasos = new long[(int) numCasos];
            for (int i = 0; i < numCasos; ++i) {
                arrayCasos[i] = sc.nextLong();
                total += arrayCasos[i];
            }
            resuelveCaso(numCasos, arrayCasos, total);
            total = 0;
            numCasos = sc.nextLong();
        }
    }

    static void resuelveCaso(long a, long[] b, long c) {
        long cont = 0, aux = c;
        for (int i = 0; i < a; i++) {
            // resto mi posicion actual
            aux -= b[i];
            // multiplico por la suma del total restante
            cont += aux * b[i];
        }
        System.out.println(cont);
    }
}
import java.io.FileWriter;
import java.util.*;
import java.util.concurrent.*;

public class Assignment2 {

    Long con = 1000000000L;

    // array list where all accounts numbers are stored to randomly create a request
    // for specific account number
    ArrayList<String> accounts_nums = new ArrayList<>();

    // thread pool array of size 10, each corresponding to one bank branch with 10
    // threads or updates.
    ExecutorService executorService[] = new ExecutorService[10];

    // hashmap where linked list of accounts of bank branches are stored.
    ConcurrentHashMap<Integer, LinkedList<Account>> banks_hashmap = new ConcurrentHashMap<Integer, LinkedList<Account>>(
            10, (float) 0.75, 100);

    public class Account {
        public String account_number; // 10 letter account number in string format with 1st letter representing bank
                                      // branch number
        public Long bank_balance;

        public Account(Long number, Long balance) {
            String s = Long.toString(number);
            while (s.length() < 10) {
                s = "0" + s;
            }
            account_number = s;
            bank_balance = balance;
        }

        public Account(String number, Long balance) {
            account_number = number;
            bank_balance = balance;
        }

    }

    // updater class with all basic operations asked to implement in assignment
    public class Updater implements Runnable {

        private int id; // id to determine which task should be done
        private String account_number;
        private Long change;
        private String acc_num2;

        public Updater(int iid) {
            id = iid;
        }

        public Updater(int iid, String acc) {
            id = iid;
            account_number = acc;
        }

        public Updater(int iid, String acc, Long bal) {
            id = iid;
            account_number = acc;
            change = bal;
        }

        public Updater(int iid, String acc, Long bal, String acc2) {
            id = iid;
            account_number = acc;
            change = bal;
            acc_num2 = acc2;
        }

        public Updater(int iid, String acc, String acc2) {
            id = iid;
            account_number = acc;
            acc_num2 = acc2;
        }

        // find index of queried account number in the linked list of the corresponding
        // bank branch
        public int findIndex(String acc, int bank_num) {
            ListIterator<Account> iter = banks_hashmap.get(bank_num).listIterator();
            int index = 0;
            Account a;
            while (iter.hasNext()) {
                a = iter.next();
                if (a.account_number == acc) {
                    index = iter.nextIndex();
                    break;
                }
            }
            index--;
            return index;
        }

        @Override
        public void run() {

            int bank_num = account_number.charAt(0) - 48;
            switch (id) {
                case 0:
                    // withdraw money from an account
                    // the account object from which the money is withdrawn is synchronized so no
                    // other thread modifies the account elements.
                    // money is withdrawn only if it is less than balance in the account
                    int index1 = findIndex(account_number, bank_num);
                    synchronized (banks_hashmap.get(bank_num).get(index1)) {
                        if (banks_hashmap.get(bank_num).get(index1).bank_balance >= change) {
                            banks_hashmap.get(bank_num).get(index1).bank_balance -= change;
                        }
                    }
                    break;

                case 1:
                    // deposit money in the account
                    // the account object to which the money should be deposited is synchronized so
                    // no other thread modifies the account elements.
                    int index2 = findIndex(account_number, bank_num);
                    synchronized (banks_hashmap.get(bank_num).get(index2)) {
                        banks_hashmap.get(bank_num).get(index2).bank_balance += change;
                    }
                    break;
                case 2:
                    // tranfer money from 1 account to other account
                    // the account object from which money is deducted is synchronized so no other
                    // thread modifies its elements
                    // after this, an independent request to thread pool of the branch to which
                    // money is transfered is submitted where it deposits the transfered money in
                    // the account number provided.
                    int index3 = findIndex(account_number, bank_num);
                    int bank_num3 = acc_num2.charAt(0) - 48;
                    synchronized (banks_hashmap.get(bank_num).get(index3)) {
                        if (banks_hashmap.get(bank_num).get(index3).bank_balance >= change) {
                            banks_hashmap.get(bank_num).get(index3).bank_balance -= change;

                        }
                    }
                    executorService[bank_num3].submit(new Updater(1, acc_num2, change));
                    break;
                case 3:
                    // add an account to a branch
                    // whole list is synchronized so that list is not modified while adding account
                    // to it.
                    synchronized (banks_hashmap.get(bank_num)) {
                        banks_hashmap.get(bank_num).add(new Account(account_number, change));
                    }
                    break;
                case 4:
                    // remove account from a branch
                    // whole list is synchronized so that list is not modified while removing
                    // account to it.
                    int index4 = findIndex(account_number, bank_num);
                    synchronized (banks_hashmap.get(bank_num)) {
                        banks_hashmap.get(bank_num).remove(index4);
                    }
                    break;
                case 5:
                    // transfer account
                    // transfer an account object from one branch to other branch with different
                    // account number.
                    int index5 = findIndex(account_number, bank_num);
                    int bank_num2 = acc_num2.charAt(0) - 48;
                    executorService[bank_num2]
                            .submit(new Updater(3, acc_num2, banks_hashmap.get(bank_num).get(index5).bank_balance));
                    synchronized (banks_hashmap.get(bank_num)) {
                        banks_hashmap.get(bank_num).remove(index5);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    public static void main(String args[]) {

        try {
            if (args.length == 0) {
                throw new Exception("tfhrt");
            }

            int total_requests = Integer.parseInt(args[0]);
            if(total_requests%1000 != 0){
                throw new Exception("tfhrt");
            }

            Assignment2 assignment2 = new Assignment2();
            assignment2.generate_accounts();
            assignment2.update_queries(args);

        } catch (Exception e) {
            System.out.println(
                    "Error while executing the program. Please provide number of requests to be generated while executing the program. Number should be a multiple of 1000.");
        }
    }

    // cretae 10000 account objects per branch for 10 branches, connect them in
    // linked list and storing the linked lists in ConcurrentHashMap
    public void generate_accounts() {

        Random rand1 = new Random();
        Random rand2 = new Random();

        try {
            // writing the account number generated and balance in them in file input.txt
            FileWriter fileWriter = new FileWriter("input.txt");

            for (int i = 0; i <= 9; i++) {

                Long acc_num = (rand1.nextLong() % con + con) % con;
                Long acc_bal = (rand2.nextLong() % con + con) % con;

                // using set so that account number is not repeated.
                Set<Long> acc_nums = new HashSet<Long>();

                // linked list of account numbers
                LinkedList<Account> bank_accounts_list = new LinkedList<Account>();

                while (acc_nums.size() < 10000) {
                    if (!acc_nums.contains(acc_num)) {
                        acc_nums.add(acc_num);

                        Account account = new Account(con * i + acc_num, acc_bal + con);
                        accounts_nums.add(account.account_number);
                        bank_accounts_list.add(account);

                        fileWriter.write("Account number " + account.account_number + " with balance "
                                + account.bank_balance + "\n");

                        acc_num = (rand1.nextLong() % con + con) % con;
                        acc_bal = (rand2.nextLong() % con + con) % con;
                    }
                }
                // placing the linkedlist in hasharray
                banks_hashmap.put(i, bank_accounts_list);
            }
            fileWriter.close();

        } catch (Exception e) {
            System.out.println("Error while executing the program.");
        }
    }

    // function to generate random queires and assigned them to appropriate thread
    // pools
    public void update_queries(String args[]) {

        // generating thread pool of 10 threads for each bank branch
        int i = 0;
        while (i < 10) {
            executorService[i] = Executors.newFixedThreadPool(10);
            i++;
        }

        // number of total requests given as input.
        int total_requests = Integer.parseInt(args[0]);

        // generating arraylist of request type ids in the proportion of total requests
        // mentioned in assignment
        ArrayList<Integer> request_types = new ArrayList<>();
        // 33% withdrawl, deposit and transfer requests
        for (i = 0; i < 33 * total_requests / 100; i++) {
            request_types.add(0);
            request_types.add(1);
            request_types.add(2);
        }
        // 0.3% add account and remove account requests
        for (i = 0; i < 3 * total_requests / 1000; i++) {
            request_types.add(3);
            request_types.add(4);
        }
        // 0.4% transfer account request
        for (i = 0; i < 4 * total_requests / 1000; i++) {
            request_types.add(5);
        }

        // randomly shuffling the request id arraylist
        Collections.shuffle(request_types);

        i = 0;

        // START TIME OF THE QUERY GENRATION AND THEIR EXECUTION
        Long a = System.currentTimeMillis();

        Random rand = new Random();
        String[] inputs = { "Deposit", "Withdraw", "Transfer", "AddAcount", "RemoveAccount", "TransferAccount" };

        try {
            // all the queries generated are written in the file queries.txt
            FileWriter fileWriter2 = new FileWriter("queries.txt");

            int temp, temp2, temp3, temp4;

            /*
             * Random query generation: As previously done, we have an arraylist of request
             * type ids. So according to the request type at ith postion, ith query is
             * genrated.
             * 
             * request type is denoted by variable temp3.
             * 
             * 1. if it is of id type 0, it is a deposit ammount request. So a random
             * integer is generated denoting amount to be deposited. Account to which amount
             * is deposited is choosen randomly by variable temp =
             * rand.nextInt(accounts_nums.size()).
             * 
             * 2. if it is of id type 1, it is withdraw amount request. So a random integer
             * is generated denoting amount to be withdrawn. Account from which amount is
             * withdrawn is choosen randomly by variable temp =
             * rand.nextInt(accounts_nums.size()).
             * 
             * 3. if it is of id type 2, it is transfer amount request. So a random integer
             * is generated denoting amount to be transfered. Account from which amount is
             * transfered and account to which amount is transfered is choosen randomly by
             * variable temp = rand.nextInt(accounts_nums.size()) and temp4 =
             * rand.nextInt(accounts_nums.size()).
             * 
             * 4. if it is of type id 3, it is add acount request. So randomly one branch is
             * choosen from 10 branches by temp4 = rand.nextInt(10) and an appropriate
             * account number is generated in 's' variable with initial amount in account
             * temp2.
             * 
             * 5. if it is of type id 4, it is remove acount request. So randomly one
             * account is choosen using temp and that account is removed from its branch
             * linkedlist.
             * 
             * 6. if it is of type id 5, it is transfer account request. So randomly one
             * account is choosen using temp which is then transfered to other branch using
             * variable temp4 with a different and appropriate account number for the new
             * branch as 1st letter of account number should reflect the bank branch.
             * 
             * Code for query generation can be seen below in respective blocks. All the
             * queries are then submitted to thread pools of bank branches corresponding to
             * the account numbers choosen.
             * 
             * All these queries are written in queries.txt
             */

            while (i < request_types.size()) {

                temp = rand.nextInt(accounts_nums.size());
                temp3 = request_types.get(i);

                if (temp3 == 0) {
                    temp2 = rand.nextInt(100000000);
                    fileWriter2.write(inputs[temp3] + " " + temp2 + " in " + accounts_nums.get(temp) + "\n");
                    int c = accounts_nums.get(temp).charAt(0) - 48;
                    executorService[c].submit(new Updater(1, accounts_nums.get(temp), Long.valueOf(temp2)));

                } else if (temp3 == 1) {
                    temp2 = rand.nextInt(1000000000) + 1000000000;
                    fileWriter2.write(inputs[temp3] + " " + temp2 + " from " + accounts_nums.get(temp) + "\n");
                    int c = accounts_nums.get(temp).charAt(0) - 48;
                    executorService[c].submit(new Updater(0, accounts_nums.get(temp), Long.valueOf(temp2)));

                } else if (temp3 == 2) {
                    temp2 = rand.nextInt(100000000);
                    temp4 = rand.nextInt(accounts_nums.size());
                    fileWriter2.write(inputs[temp3] + " " + temp2 + " from " + accounts_nums.get(temp) + " to "
                            + accounts_nums.get(temp4) + "\n");
                    int c = accounts_nums.get(temp).charAt(0) - 48;
                    executorService[c].submit(
                            new Updater(2, accounts_nums.get(temp), Long.valueOf(temp2), accounts_nums.get(temp4)));

                } else if (temp3 == 3) {
                    temp4 = rand.nextInt(10);
                    Long temp5 = Long.valueOf(temp4) * con + Long.valueOf(rand.nextInt(1000000000));
                    String s = Long.toString(temp5);
                    while (s.length() < 10) {
                        s = "0" + s;
                    }
                    temp2 = rand.nextInt(1000000000) + 1000000000;
                    accounts_nums.add(s);
                    fileWriter2.write(inputs[temp3] + " to branch " + String.valueOf(temp4) + " with account number "
                            + s + " and balance " + temp2 + "\n");
                    int c = s.charAt(0) - 48;
                    executorService[c].submit(new Updater(3, s, Long.valueOf(temp2)));

                } else if (temp3 == 4) {
                    fileWriter2.write(inputs[temp3] + " " + accounts_nums.get(temp) + "\n");
                    int c = accounts_nums.get(temp).charAt(0) - 48;
                    executorService[c].submit(new Updater(4, accounts_nums.get(temp)));
                    accounts_nums.remove(temp);

                } else {
                    temp4 = rand.nextInt(10);
                    String temp7 = accounts_nums.get(temp);
                    int temp6 = temp7.charAt(0) - 48;
                    while (temp4 == temp6) {
                        temp4 = rand.nextInt(10);
                    }
                    Long temp5 = Long.valueOf(temp4) * con + Long.valueOf(rand.nextInt(1000000000));
                    String s = Long.toString(temp5);
                    while (s.length() < 10) {
                        s = "0" + s;
                    }
                    ;
                    executorService[temp6].submit(new Updater(5, temp7, s));
                    fileWriter2.write(inputs[temp3] + " from account number " + temp7 + " to " + s + "\n");
                }
                i++;
            }
            fileWriter2.close();

        } catch (Exception e) {
            System.out.println("Error while executing the program.");
        }

        i = 0;
        // checking whether thread pools are terminated or not. If not, waiting for them
        // to terminate.
        while (i < 10) {
            executorService[i].shutdown();
            while (!executorService[i].isTerminated())
                ;
            i++;
        }

        // ENDING TIME OF EXECUTION OF ALL QUERIES GENERATED.
        Long b = System.currentTimeMillis();

        // PRINTING THE AMOUNT OF TIME TAKEN.
        System.out.println("Time taken by program to execute "+ String.valueOf(total_requests/100) + " queries per updater is " +  String.valueOf((b-a)/1000f) + " seconds." );

        try {
            // Printing account number and balance in them for all accounts in all branches.
            FileWriter fileWriter1 = new FileWriter("output.txt");

            for (int h = 0; h < 10; h++) {
                fileWriter1.write("Bank " + h + "\n");
                ListIterator<Account> iter = banks_hashmap.get(h).listIterator();
                while (iter.hasNext()) {
                    Account ac = iter.next();
                    fileWriter1.write("Account " + ac.account_number + " has balance " + ac.bank_balance + "\n");
                }
            }
            fileWriter1.close();

        } catch (Exception e) {
            System.out.println("Error while executing the program.");
        }
    }
}
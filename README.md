<h1>CustomDB is A light-weight Key-Value Database System</h1>

CustomDB is a key-value storage library and it use both <a href="http://en.wikipedia.org/wiki/Hash_chain">HashChain</a> and <a href="http://en.wikipedia.org/wiki/Extendible_hashing">Extendible_hashing</a> as internel structure. Also, I have implemented LRU-Cache and FIFO-Cache in its external Cache-System. Anyone, who has interest in it, can add self-customed structure or Cache-Schedule Algorithm to enhance its ability. That is the reason I renamed it as <span style="color:red">CustomDB</span>.

Our work is mainly inspired by <a href="www.apuebook.com/‎">APUE</a>, <a href="www.gnu.org/s/gdbm/‎">GDBM</a>, <a href="https://code.google.com/p/leveldb/">leveldb</a> and <a href="https://github.com/kedebug/yodb">yodb</a>. Thanks again for open-source group.

<h2>Features</h2>
<ul>
   <li>Keys and values are arbitrary byte arrays.</li>
   <li>The basic operations are Put(key,value), Get(key), Delete(key).</li>
   <li>Custom-option Ability, like turn on/off log. As limitation of time, I don't provide enough implementation for different option. Any one who has interest in it can try it. Very simple.</li>
   <li>Exposed DBImpl(database implementation) interface and Cache interface.</li>
   <li>Some blogs of this project(under construction) about how to use the library and the design principle of this library.</li>
   <li>Little RAM requirment.</li>
   <li>Small Index requirment(Compared with leveldb).</li>
</ul>

<h2>Limitations</h2>
<ul>
   <li>This is not a SQL database. It does not have a relational data model, it does not support SQL queries, and it has no support for indexes.</li>
   <li>Only a single process can access a particular database at a time. However, multiple-threading can be accepted. For the detail, read db_batch_thread.cpp and db_parallel_thread.cpp in detail.</li>
   <li>Only localhost db is supported.</li>
   <li>I doesn't provide any compression tool in this db. Therefore, when the etries in very large(like 10 Million and each entry has 100 bytes), the space requirement is very large.</li>
   <li>It only supports random write and random read(As limitation of HashTable).</li>
</ul>

<h2>Performance</h2>
To test our library, I used the test suit of leveldb and succeed to test CustomDB's performance.s

<h3>Setup</h3>
We tried our database with a million entries. Each entry has a 16 byte key, and a 100 byte value. Too get maxinum performance, we turned off log and cache(As the space of the key is far larger than we put, the cache has little performance balance.)

```
CustomDB:   version 1.0
Date:       Wed Feb 19 21:55:09 2014
CPU:        4 * Intel(R) Core(TM) i5-3317U CPU @ 1.70GHz
CPUCache:   3072 KB
Keys:       16 bytes each
Values:     100 bytes each
Entries:    1000000
RawSize:    150.0 MB (estimated)
FileSize:   116.0 MB (estimated)
WARNING:    Log is disabled, for better benchmarks
WARNING:    Cache is disabled, for better benchmarks.
```

<h3>Write performance</h3>
The "fill" benchmarks create a brand new database, in random order. The "fillsync" benchmark flushes data from the operating system to the disk after every operation; the other write operations leave the data sitting in the operating system buffer cache for a while.

```
fillsync        :       55.0 us/op;   2.01 MB/s     
fillbatch       :       10.0 us/op;   16.1 MB/s 
fillthreadbatch :       18.0 us/op;   8.88 MB/s
fillthreadbatch :       21.0 us/op;   7.61 MB/s
```
Each "op" above corresponds to a read/write of a single key/value pair. 

<h3>Read performance</h3>
We list the performance of a random lookup. Note that the database created by the benchmark is quite small. Therefore the report characterizes the performance of leveldb when the working set fits in memory. Before this test, I have clear the cache generated by put operation.
```
readrandom      :       19.0 us/op;   8.42 MB/s  
```
<h3>Compression Ration</h3>
CustomDB provide compact feature to compression raw database. However, it only can compress fillsync, which is the most application situation. That can be called time-memory trade-off technology.
```
            Before Compression  |  After Compression
Raw Size:   230 MB                   131 MB
```
<h2>Usage</h2>
I just tested it on Ubuntu 13.10, so if you have any problem, please feel free to send email to me.

<h3>Compile and Installation</h3>
CustomDB use GNU Make to handle compilation, you can find detail information from <a href="www.gnu.com/Make/">this url</a>.
```
$ make
$ make tests
$ make db_tests
$ sudo make install
```
<h3>Detail Usage</h3>
For better and intutive demo, please read demos in directory ```$(PROJECTDIR)/tests```.

<h4>Open & Close CustomDB</h4>

```c++
   Option option; //Detail? Read Source Code. 
   CustomDB db = new CustomDB;
   db -> init(option);  
   db -> close();
   delete db;
```

<h4>Dump Into Terminal</h4>

```c++
   Option option;
   CustomDB db = new CustomDB;
   db -> init(option);  
   db -> dump();
   db -> close();
   delete db;
```
<h4>Put Elementse</h4>

```c++
   Slice key("slice"); //Slice is default method to manpiate the item in the database.
   Slice value("123213");
   db -> put(key, value); // Return value will give out whether we put it successfully.
   db -> put("123213","iipppp"); //Another way to Put elements in it.
```

<h4>Get Elements</h4>
```c++
   Slice key("slice"); 

   Slice value = db -> get(key); 
   assert(value.size() != 0); //Not empty.
```

<h4>Remove Elements</h4>
```c++
   Slice key("slice");
   bool successful = db -> remove(key);
```

<h4>Batch Write</h4>
```c++
   WriteBatch batch(3);
   batch.put(k1, k2);
   batch.put(k2, k3);
   batch.put(k3, k1);
   db -> write(&batch);
```

<h4>Multiple_Threading Writting</h4>
```c++
   WriteBatch batch(3);
   batch.put(k1, k2);
   batch.put(k2, k3);
   batch.put(k3, k1);
   db -> write(&batch);
   db -> runBatchParallel(&batch);
```
![CustomDB UML Graph](https://raw.githubusercontent.com/mathewes/blog-dot-file/master/CustomDB.png)
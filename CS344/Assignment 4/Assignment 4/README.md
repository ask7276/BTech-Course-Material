Step 1:
Create partitions with the following names:
1) ext4sample (ext4)
2) mp (zfs pool)

Creation is mentioned in the report.

Step 2:
(a) To test data deduplication:  
    //To set the configuration
    sudo zfs dedup=on mp
    sudo zfs compression=off mp
    
    sudo ./vdbench ToTestDedupzfs -o dedupzfs
    zpool list

    sudo ./vdbench ToTestDedupext4 -o dedupext4

(b) To test data compression:  
    //To set the configuration
    sudo zfs dedup=off mp
    sudo zfs compression=on mp
    
    sudo ./vdbench ToTestCompzfs -o compzfs
    zpool list

    sudo ./vdbench ToTestCompext4 -o compext4

(c) To test read write:  
    //To set the configuration
    sudo zfs dedup=off mp
    sudo zfs compression=off mp

    sudo ./vdbench ToTestReadWritezfs -o rwzfs
    zpool list

    sudo ./vdbench ToTestReadWriteExt4 -o rwext4

About the workload:
rdpct=50%===> That 50% opeartions are read and 50% are write
the directory structure is as follows

                  Root
                /      \
               ()       ()
              /  \     /  \
            ()() ()() ()() ()()
At the leaves are files i.e. 8 files in total(50m each)
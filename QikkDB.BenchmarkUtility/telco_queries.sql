SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.1303 AND latitude < 48.1391 AND longitude > 17.0779 AND longitude < 17.0912 GROUP BY ageId;
9999
SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.1395 AND latitude < 48.1573 AND longitude > 17.0798 AND longitude < 17.0952 GROUP BY ageId;
9999
SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.1415 AND latitude < 48.1534 AND longitude > 17.0894 AND longitude < 17.1027 GROUP BY ageId;
9999
SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.1512 AND latitude < 48.1612 AND longitude > 17.0996 AND longitude < 17.1126 GROUP BY ageId;
9999
SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.1549 AND latitude < 48.1708 AND longitude > 17.1033 AND longitude < 17.1193 GROUP BY ageId;
9999
SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.1592 AND latitude < 48.1785 AND longitude > 17.1116 AND longitude < 17.1312 GROUP BY ageId;
9999
SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.1689 AND latitude < 48.1799 AND longitude > 17.1202 AND longitude < 17.1388 GROUP BY ageId;
9999
SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.179  AND latitude < 48.1977 AND longitude > 17.1218 AND longitude < 17.1397 GROUP BY ageId;
9999
SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.1868 AND latitude < 48.1991 AND longitude > 17.1299 AND longitude < 17.1411 GROUP BY ageId;
9999
SELECT COUNT(ageId) FROM TargetLoc1B WHERE latitude > 48.1946 AND latitude < 48.2142 AND longitude > 17.1386 AND longitude < 17.1512 GROUP BY ageId;
9999
SELECT sum(wealthIndexId) AS sum_wealthIndexId FROM TargetLoc1B WHERE (latitude - longitude) < 15;
9999
SELECT min(longitude), max(latitude) FROM TargetLoc1B;
9999
SELECT hwOsId, AVG(ageId) FROM TargetLoc1B WHERE genderId != -1 GROUP BY hwOsId;
9999
SELECT genderId, COUNT(genderId) FROM TargetLoc1B WHERE ROUND(POW(SQRT(longitude), 2)) = ROUND(longitude) GROUP BY genderId;
9999
SELECT genderId, COUNT(genderId) FROM TargetLoc1B WHERE ROUND(POW(SQRT(longitude), 2)) = ROUND(SQUARE(ROOT(longitude, 2))) GROUP BY genderId;
9999
SELECT MAX(ageId * PI()) FROM TargetLoc1B WHERE ROUND(POW(latitude, 2)) > 5 GROUP BY ageId;
9999
SELECT genderId, MAX(ageId), MIN(wealthIndexId) FROM TargetLoc1B GROUP BY genderId;
9999
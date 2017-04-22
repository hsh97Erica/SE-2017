-- MySQL dump 10.13  Distrib 5.7.17, for macos10.12 (x86_64)
--
-- Host: localhost    Database: logindb
-- ------------------------------------------------------
-- Server version	5.7.17

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `accountlist`
--

DROP TABLE IF EXISTS `accountlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `accountlist` (
  `id` varchar(2048) CHARACTER SET latin1 NOT NULL,
  `pw` varchar(8192) CHARACTER SET latin1 NOT NULL DEFAULT '',
  `accesslevel` int(11) DEFAULT NULL,
  `etc` blob,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accountlist`
--

LOCK TABLES `accountlist` WRITE;
/*!40000 ALTER TABLE `accountlist` DISABLE KEYS */;
INSERT INTO `accountlist` VALUES ('admin','admin123',0,NULL),('hsh','971119',3,NULL),('hsh97','971119',3,NULL);
/*!40000 ALTER TABLE `accountlist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary view structure for view `availableloginlist`
--

DROP TABLE IF EXISTS `availableloginlist`;
/*!50001 DROP VIEW IF EXISTS `availableloginlist`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `availableloginlist` AS SELECT 
 1 AS `id`,
 1 AS `pw`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `availableloginlist2`
--

DROP TABLE IF EXISTS `availableloginlist2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `availableloginlist2` (
  `id` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `availableloginlist2`
--

LOCK TABLES `availableloginlist2` WRITE;
/*!40000 ALTER TABLE `availableloginlist2` DISABLE KEYS */;
INSERT INTO `availableloginlist2` VALUES ('tmp3'),('tmp2'),('admin');
/*!40000 ALTER TABLE `availableloginlist2` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `loginhash`
--

DROP TABLE IF EXISTS `loginhash`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `loginhash` (
  `id` varchar(2048) NOT NULL,
  `hashv` varchar(8192) NOT NULL,
  `ip` text CHARACTER SET utf8,
  `etc` blob,
  KEY `id` (`id`),
  CONSTRAINT `loginhash_ibfk_1` FOREIGN KEY (`id`) REFERENCES `accountlist` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `loginhash`
--

LOCK TABLES `loginhash` WRITE;
/*!40000 ALTER TABLE `loginhash` DISABLE KEYS */;
INSERT INTO `loginhash` VALUES ('admin','975fad5124e082cabc80370021eb3360',NULL,NULL),('admin','a72d01c88b5961ff15536fcef39bf382',NULL,NULL),('admin','63cf149d62f564d690203534a9fc301c',NULL,NULL),('admin','d5ee6d5b6a8ebd63cb277d5547421c08',NULL,NULL),('admin','a1d39c92f783b5e762c1c90165f81ff7',NULL,NULL),('admin','7e88bae82593e971805ca9413e46f95b',NULL,NULL),('admin','be5ab8fb3c072ac095bd5c74d668b723',NULL,NULL),('admin','8cf1aab84b12fac9f7f0c1469f576871',NULL,NULL),('admin','f37721ff48f3555426b124a7ae0785ca',NULL,NULL),('admin','c36f1345c0deb405494754c455d89e65',NULL,NULL),('admin','c8bfbff22d650969cdff576b58935d3b',NULL,NULL),('admin','3046b5831f7a104e3e512df3ef6ebb1d',NULL,NULL),('admin','9d72b46dcc72a15aaab72348a2696d38',NULL,NULL),('admin','8b848f79b9e406765a53b1d9fe4db4d0',NULL,NULL),('admin','9eff7943222b87d926174461dbf56ed3',NULL,NULL),('admin','78240cc1f1d857979483823b50516731',NULL,NULL),('admin','cb92f1fe48d69608812a8275171b4c9a',NULL,NULL),('admin','775410ea30b4f77e752e92b068de86b9',NULL,NULL),('admin','5ae75dc18f0ef4ddc10bacc8ac68e02a',NULL,NULL),('admin','0bcf8258cb6f2f0db917bfe28a70c77a',NULL,NULL),('admin','0bcf8258cb6f2f0db917bfe28a70c77a',NULL,NULL),('admin','ea1326c9c8774b091e267f335c79a938',NULL,NULL),('admin','b6b28ef5e3750d55c1a7d2dcb34cc84f',NULL,NULL),('admin','85da9f97f34932aaedcc733603fcfa8a',NULL,NULL),('admin','7dee3cd978061d9e2a928119ea2d48ce',NULL,NULL),('admin','c391e6ab51423d3b4d374cef7645e755',NULL,NULL),('hsh97','4673c61a6e571d727d5f8ac17bbe4f3b',NULL,NULL),('hsh','2561cc5e117bdea7e023ca40fe3708e4',NULL,NULL),('admin','d031b1235560225883d0e5c130fa49a7',NULL,NULL);
/*!40000 ALTER TABLE `loginhash` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `loginhash2`
--

DROP TABLE IF EXISTS `loginhash2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `loginhash2` (
  `id` text CHARACTER SET latin1 NOT NULL,
  `hashd` text CHARACTER SET latin1 NOT NULL,
  `starttime` bigint(30) NOT NULL,
  `expire` bigint(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `loginhash2`
--

LOCK TABLES `loginhash2` WRITE;
/*!40000 ALTER TABLE `loginhash2` DISABLE KEYS */;
INSERT INTO `loginhash2` VALUES ('tmp2','095026c12aa935d7d2d3482debcf0ab6',1491581191365,1491624391365),('tmp2','61fc2153e5fc41b3866d36c0124b975e',1491581310670,1491624510670),('tmp2','10460781a05985ba656e346c94963ee2',1491581349434,1491624549434),('tmp2','112d57c8ecc2b375828faf4528d88e81',1491581463718,1491624663718);
/*!40000 ALTER TABLE `loginhash2` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_info`
--

DROP TABLE IF EXISTS `user_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_info` (
  `id` varchar(2048) NOT NULL,
  `nickname` varchar(2048) NOT NULL,
  `gender` int(11) DEFAULT NULL,
  `etc` blob,
  PRIMARY KEY (`id`),
  CONSTRAINT `user_info_ibfk_1` FOREIGN KEY (`id`) REFERENCES `accountlist` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_info`
--

LOCK TABLES `user_info` WRITE;
/*!40000 ALTER TABLE `user_info` DISABLE KEYS */;
INSERT INTO `user_info` VALUES ('admin','xeon97',NULL,NULL),('hsh','xero',NULL,NULL),('hsh97','hsh97',NULL,NULL);
/*!40000 ALTER TABLE `user_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Final view structure for view `availableloginlist`
--

/*!50001 DROP VIEW IF EXISTS `availableloginlist`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `availableloginlist` AS select `accountlist`.`id` AS `id`,`accountlist`.`pw` AS `pw` from `accountlist` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-04-22 10:15:26

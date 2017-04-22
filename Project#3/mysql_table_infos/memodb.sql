-- MySQL dump 10.13  Distrib 5.7.17, for macos10.12 (x86_64)
--
-- Host: localhost    Database: memodb
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
-- Table structure for table `memocontent`
--

DROP TABLE IF EXISTS `memocontent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `memocontent` (
  `boardnumber` int(11) NOT NULL,
  `passwordlocked` varchar(2048) DEFAULT NULL,
  `title` varchar(4096) NOT NULL,
  `memodata` blob NOT NULL,
  `create_date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `modify_date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`boardnumber`),
  CONSTRAINT `memocontent_ibfk_1` FOREIGN KEY (`boardnumber`) REFERENCES `memolist` (`boardnumber`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `memocontent`
--

LOCK TABLES `memocontent` WRITE;
/*!40000 ALTER TABLE `memocontent` DISABLE KEYS */;
INSERT INTO `memocontent` VALUES (1,NULL,'new memo in add mode','<p class=\"style-scope wysiwyg-e\">this memo is added by add mode&nbsp;</p><p class=\"style-scope wysiwyg-e\">:)</p><p class=\"style-scope wysiwyg-e\">and remain data on editor view ?</p>','2017-04-09 13:33:43','2017-04-10 21:02:32'),(5,NULL,'test','<p class=\"style-scope wysiwyg-e\"><b>test12333</b></p><p class=\"style-scope wysiwyg-e\"><b>ㄴㅁㅇd</b></p><p class=\"style-scope wysiwyg-e\"><b>213123</b></p>','2017-04-10 13:23:05','2017-04-10 21:04:22'),(7,NULL,'리프레시됨?','<p class=\"style-scope wysiwyg-e\">리프레시가 되나요?</p><p class=\"style-scope wysiwyg-e\">에엑! 업뎃이 안되나요 :</p><p class=\"style-scope wysiwyg-e\">왜 안되나요?</p><p class=\"style-scope wysiwyg-e\">base64로 암호화래 업데이트가 제대로 되나 시험</p>','2017-04-10 14:03:35','2017-04-21 22:57:56');
/*!40000 ALTER TABLE `memocontent` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `memolist`
--

DROP TABLE IF EXISTS `memolist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `memolist` (
  `id` varchar(2048) NOT NULL,
  `boardnumber` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`boardnumber`),
  KEY `id` (`id`),
  CONSTRAINT `memolist_ibfk_1` FOREIGN KEY (`id`) REFERENCES `logindb`.`accountlist` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `memolist`
--

LOCK TABLES `memolist` WRITE;
/*!40000 ALTER TABLE `memolist` DISABLE KEYS */;
INSERT INTO `memolist` VALUES ('admin',1),('admin',5),('admin',7),('admin',10);
/*!40000 ALTER TABLE `memolist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary view structure for view `memoview`
--

DROP TABLE IF EXISTS `memoview`;
/*!50001 DROP VIEW IF EXISTS `memoview`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `memoview` AS SELECT 
 1 AS `id`,
 1 AS `boardnumber`,
 1 AS `passwordlocked`,
 1 AS `title`,
 1 AS `memodata`,
 1 AS `create_date`,
 1 AS `modify_date`*/;
SET character_set_client = @saved_cs_client;

--
-- Final view structure for view `memoview`
--

/*!50001 DROP VIEW IF EXISTS `memoview`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `memoview` AS (select `ml`.`id` AS `id`,`ml`.`boardnumber` AS `boardnumber`,`mc`.`passwordlocked` AS `passwordlocked`,`mc`.`title` AS `title`,`mc`.`memodata` AS `memodata`,`mc`.`create_date` AS `create_date`,`mc`.`modify_date` AS `modify_date` from (`memolist` `ml` join `memocontent` `mc`) where (`ml`.`boardnumber` = `mc`.`boardnumber`)) */;
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

-- Dump completed on 2017-04-22 10:16:23

<?php
// script servant uniquement à permettre à ce que l'utilisateur puisse faire oublier sa session au serveur
session_start();
session_unset();
?>
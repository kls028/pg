<?php
require_once '../views/Error404View.php';

class ErrorController {
    public function _404() {
        $view = new Error404View();
        $view->render();
    }
}

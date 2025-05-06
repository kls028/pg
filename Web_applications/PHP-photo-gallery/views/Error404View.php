<?php

class Error404View{
    public function render() {
        http_response_code(404);
        echo '<!DOCTYPE html>';
        echo '<html lang="en">';
        echo '<head>';
        echo '<meta charset="UTF-8">';
        echo '<meta name="viewport" content="width=device-width, initial-scale=1.0">';
        echo '<title>404 - Not Found</title>';
        echo '</head>';
        echo '<body>';
        echo '<h1>404 - Page Not Found</h1>';
        echo '<p>Sorry, the page you are looking for does not exist.</p>';
        echo '</body>';
        echo '</html>';
    }
}
<!-- app/views/gallery.php -->
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Your Gallery - Sudoku Gallery</title>
    <link rel="stylesheet" href="styles/gallery-style.css">
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
</head>
<body>
    <h2>Sudoku Gallery</h2>
    <div class="search-container">
        <input type="text" id="search-input" placeholder="Search images..." oninput="searchImages()">
    </div>
    <div class="navbar">
        <!-- Left side: counter -->
        <div class="navbar-left">
            <p>Images added: <?php echo $publicImageCount; ?></p>
        </div>

        <!-- Center: Upload Link -->
        <div class="navbar-center">      
            <a href="app.php?controller=gallery&action=uploadAction" class="navbar-link">Upload New Image</a>
        </div>

        <!-- Right side: Profile section -->
        <div class="navbar-right">
            <?php if (!isset($user)): ?>
                <a href="app.php?controller=user&action=loginAction" class="navbar-link">Login</a>
            <?php else: ?>
                <div class="profile-dropdown">
                    <span class="profile-name"><?php echo htmlspecialchars($user); ?></span>
                    <div class="dropdown-content">
                        <a href="app.php?controller=user&action=logoutAction">Logout</a>
                    </div>
                </div>
            <?php endif; ?>
        </div>
    </div>
   


    <form action="app.php?controller=gallery&action=showSelectedAction" method="POST">
                
    <button type="submit" class="select-btn">Show selected</button>

    <div id="title-div"><h3>Public Images</h3></div>
    <div class="gallery-container">
    <?php foreach ($publicImages as $image): ?>
        <div class="image-card">
        <a href="app.php?controller=gallery&action=viewImageAction&filename=<?php echo urlencode($image['filename']); ?>">
            <img src="/images/<?php echo basename($image['filename']); ?>" alt="Image" width="200">   
        </a>
        <h3><?php echo $image['title']; ?></h3>
        <p>by: <?php echo $image['author']; ?></p>
        <input type="checkbox" 
               name="selected_images[]" 
               value="<?php echo $image['_id']; ?>" 
               <?php echo isset($_SESSION['selected_images']) && in_array($image['_id'], $_SESSION['selected_images']) ? 'checked' : ''; ?>>
        
        </div>
    <?php endforeach; ?>
    </div>
    </form>
    <div class="pagination-container">
        <div class="pagination">
            <a href="app.php?controller=gallery&action=galleryAction&page=<?php echo $page - 1; ?>" 
                <?php echo $page <= 1 ? 'class="disabled"' : ''; ?>>Previous</a>
            <span>Page <?php echo $page; ?></span>
            <a href="app.php?controller=gallery&action=galleryAction&page=<?php echo $page + 1; ?>" 
                <?php echo count($publicImages) < 8 ? 'class="disabled"' : ''; ?>>Next</a>
        </div>
    </div>


    <?php if(isset($user)): ?>
    <div id="title-div"><h3>Your Private Images</h3></div>
    <div class="gallery-container" id="gallery-container">
        <?php if (isset($user)): ?>
            
            <?php if ($privateImageCount) : ?>
                <?php foreach ($privateImages as $image): ?>
            
                <div class="image-card">
                    <a href="app.php?controller=gallery&action=viewImageAction&filename=<?php echo urlencode($image['filename']); ?>">
                        <img src="/images/<?php echo basename($image['filename']); ?>" alt="Image" width="200">
                    </a>
                    <h3><?php echo $image['title']; ?></h3>
                    <form action="app.php?controller=gallery&action=deleteImageAction" method="POST">
                        <input type="hidden" name="image_id" value="<?php echo $image['_id']; ?>"> <!-- MongoDB ObjectId -->
                        <button class="delete-btn" type="submit">Delete</button>
                    </form>
                </div>
                <?php endforeach; ?>
            <?php else:?>
                <p>You have not uploaded any private images yet.</p>
            <?php endif;?>
        <?php endif; ?>
    <?php endif; ?>
        
    </div>

    

</body>
<script>
    function searchImages() {
        var searchQuery = document.getElementById('search-input').value;

        $.ajax({
            url: 'app.php?controller=gallery&action=searchAction',
            type: 'GET',
            data: { query: searchQuery },
            success: function(response) {
                document.querySelector('.gallery-container').innerHTML = response;
            },
            error: function() {
                alert("Error while fetching images.");
            }
        });
    }
</script>

</html>

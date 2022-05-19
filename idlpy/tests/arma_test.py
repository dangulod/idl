import unittest
import idlpy
import idlpy.testing

class TestCube(unittest.TestCase):
    rows      = 2
    columns   = 3
    slices    = 4
    arma_cube = idlpy.testing.test_arma_cube(rows, columns, slices)
    np_cube   = arma_cube.to_numpy()

    def test_len(self):
        self.assertEqual((self.arma_cube.n_slices, self.arma_cube.n_rows, self.arma_cube.n_cols), 
                          self.np_cube.shape)
        self.assertEqual(len(self.arma_cube), self.rows * self.columns * self.slices)

    def test_get_item(self):
        for it_slice in range(0, self.arma_cube.n_slices):
            for it_row in range(0, self.arma_cube.n_rows):
                for it_column in range(0, self.arma_cube.n_cols):
                    self.assertEqual(self.arma_cube[it_row, it_column, it_slice],
                                     self.np_cube[it_slice, it_row, it_column])
                    self.assertEqual(self.arma_cube[it_slice][it_row][it_column],
                                     self.np_cube[it_slice][it_row][it_column])

    def test_slices(self):
        for it_slice in range(0, self.arma_cube.n_slices):
            self.assertEqual((self.arma_cube.slice(it_slice).to_numpy() == \
                self.np_cube[it_slice]).sum(), self.rows * self.columns)
    
    def test_rows(self):
        for it_row in range(0, self.arma_cube.n_rows):
            self.assertEqual((self.arma_cube.row(it_row).to_numpy() == \
                self.np_cube[:, it_row, :]).sum(), self.slices * self.columns)

    def test_cols(self):
        for it_col in range(0, self.arma_cube.n_cols):
            self.assertEqual((self.arma_cube.col(it_col).to_numpy() == \
                self.np_cube[:, :, it_col]).sum(), self.rows * self.slices)


class TestMatrix(unittest.TestCase):
    rows        = 2
    columns     = 3
    arma_matrix = idlpy.testing.test_arma_matrix(rows, columns)
    np_matrix   = arma_matrix.to_numpy()

    def test_len(self):
        self.assertEqual((self.arma_matrix.n_rows, self.arma_matrix.n_cols), 
                          self.np_matrix.shape)
    
    def test_get_item(self):
        for it_row in range(0, self.arma_matrix.n_rows):
            for it_column in range(0, self.arma_matrix.n_cols):
                self.assertEqual(self.arma_matrix[it_row, it_column],
                                 self.np_matrix[it_row, it_column])
                self.assertEqual(self.arma_matrix[it_row][it_column],
                                 self.np_matrix[it_row][it_column])
    
    def test_row(self):
        for it_row in range(0, self.arma_matrix.n_rows):
            self.assertEqual((self.arma_matrix.row(it_row).to_numpy() == \
                self.np_matrix[it_row, :]).sum(), self.columns)

    def test_col(self):
        for it_col in range(0, self.arma_matrix.n_cols):
            self.assertEqual((self.arma_matrix.col(it_col).to_numpy() == \
                self.np_matrix[:, it_col]).sum(), self.rows)
    
    def test_t(self):
        t_arma_matrix = self.arma_matrix.t()
        t_np_matrix   = self.np_matrix.T
        for it_row in range(0, t_arma_matrix.n_rows):
            for it_column in range(0, t_arma_matrix.n_cols):
                self.assertEqual(t_arma_matrix[it_row, it_column],
                                 t_np_matrix[it_row, it_column])
                self.assertEqual(t_arma_matrix[it_row][it_column],
                                 t_np_matrix[it_row][it_column])

class TestVector(unittest.TestCase):
    n        = 20
    arma_vec = idlpy.testing.test_arma_vec(n)
    np_vec   = arma_vec.to_numpy()

    def test_len(self):
        self.assertEqual((self.arma_vec.n_rows, ), 
                          self.np_vec.shape)
        self.assertEqual(len(self.arma_vec), 
                         len(self.np_vec))
    
    def test_get_item(self):
        for it_n in range(0, self.arma_vec.n_cols):
            self.assertEqual(self.arma_vec[it_n],
                                self.np_vec[it_n])
            self.assertEqual(self.arma_vec[it_n],
                                self.np_vec[it_n])

    def test_iter(self):
        for it_arma, it_np in zip(self.arma_vec, self.np_vec):
            self.assertEqual(it_arma,
                             it_np)

if __name__ == '__main__':
    unittest.main()
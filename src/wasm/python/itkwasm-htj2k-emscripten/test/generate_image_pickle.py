import pickle

if __name__ == '__main__':
    from pathlib import Path
    input_base_path = Path('..', '..', 'test', 'data')
    test_files = [
        Path('input') / 'itk_logo.png',
        Path('input') / 'CT1.mha',
    ]
    data = {}
    import itk
    from itkwasm import Image
    for f in test_files:
        path = input_base_path / f
        image = itk.imread(path)
        image_dict = itk.dict_from_image(image)
        image_dict['metadata'] = dict()
        print(str(path))
        with open(f'{str(path)}.pickle', 'wb') as fp:
            # Pickle dict representation for use in pyodide test to avoid
            # bootstrapping conflict with itkwasm_image_io
            pickle.dump(image_dict, fp)
from architectds import * 
import os

class GenericConversion(GenericBinary):

  def __init__(
      self, 
      name,
      in_ext,
      command_generator = (lambda x,y : f"echo {x} and {y}") ,base_output_dir = 'build/',
  ):
    self.ASSET_BARRIER_FLAG = "assets_" + name + "_files"
    self.name = name
    super().__init__(self.ASSET_BARRIER_FLAG)
    self.out_assets_path = base_output_dir
    self.add_dir_target(self.out_assets_path)
    self.command_generator = command_generator
    self.in_ext = in_ext
    self.target_files = []
  
  def add_conversion(self, in_dirs, out_dir, out_ext="bin"):
    full_out_dir = os.path.join(self.out_assets_path, out_dir)

    in_out_files = []
    
    for in_dir in in_dirs:
      in_files = gen_input_file_list(in_dir, self.in_ext)
      in_out_files.extend(gen_out_file_list(in_files, in_dir, full_out_dir, '', out_ext))

    for in_out_file in in_out_files:
      out_path_dir = get_parent_dir(in_out_file.out_path)
      self.add_dir_target(out_path_dir)
            
      in_path = in_out_file.in_path
      out_path = in_out_file.out_path
      self.target_files.append(out_path)
      self.print(
        f'build {out_path}: {self.name}_convert {in_path} || {out_path_dir}\n'
        '  command = ' + self.command_generator(in_path, out_path) + "\n"
        '\n'
      )
  
  def _gen_rule_assets_barrier(self):
    '''
        This generates a common phony target to all the files inside the
        filesystem. This phony target can be used instead of all the files when
        another target depends on the filesystem as a whole, like the NDS ROM.
    '''
    flag_path = self.flag_assets_name
    file_paths_str = ' '.join(self.target_files)
    self.print(
        f'build {flag_path}: phony {file_paths_str}\n'
        '\n'
    )

  def _gen_rules_tools(self):
    self.print(
      f'rule {self.name}_convert\n'
      '  command = ' + self.command_generator('$in', '$out') + '\n'
      '\n'
    )
    
  def generate_image(self):
    self._gen_rule_assets_barrier()
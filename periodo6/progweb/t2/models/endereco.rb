require 'active_record'

class Endereco < ActiveRecord::Base
    belongs_to :cliente
end
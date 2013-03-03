#ifndef __itkImpulseNoiseImageFilter
#define __itkImpulseNoiseImageFilter

#include <itkUnaryFunctorImageFilter.h>
#include <itkConceptChecking.h>
#include <vnl/vnl_sample.h>

namespace itk
{
namespace Functor
{
/** \class ImpulseNoise
 * \brief Adds impulse noise to a pixel.
 * \ingroup ITKImageIntensity
 */
template< class TInput, class TOutput >
class ImpulseNoise
{
public:
  ImpulseNoise() {
    m_Probability = 0.01;
    m_OutputMinimum = itk::NumericTraits< TOutput >::min();
    m_OutputMaximum = itk::NumericTraits< TOutput >::max();
  }
  ~ImpulseNoise() {}

  double GetProbability() const { return m_Probability; }
  void SetProbability(double probability) { m_Probability = probability; }

  TOutput GetOutputMinimum() const { return m_OutputMinimum; }
  TOutput GetOutputMaximum() const { return m_OutputMaximum; }

  void SetOutputMinimum( TOutput min ) { m_OutputMinimum = min; }
  void SetOutputMaximum( TOutput max ) { m_OutputMaximum = max; }

  bool operator!=(const ImpulseNoise &other) const
  {
    return m_Probability != other.m_Probability
           || m_OutputMaximum != other.m_OutputMaximum
           || m_OutputMinimum != other.m_OutputMinimum;
  }

  bool operator==(const ImpulseNoise & other) const
  {
    return !( *this != other );
  }

  inline TOutput operator()(const TInput & A) const
  {
    if(vnl_sample_uniform(0., 1.) < m_Probability)
      {
      if(vnl_sample_uniform(0., 1.) < 0.5)
        return static_cast<TOutput>(m_OutputMinimum);
      else
        return static_cast<TOutput>(m_OutputMaximum);
      }
    else
      return static_cast<TOutput>(A);
  }

private:
  TOutput m_OutputMinimum;
  TOutput m_OutputMaximum;
  double m_Probability;
};
}

template< class TInputImage, class TOutputImage >
class ITK_EXPORT ImpulseNoiseImageFilter:
  public
  UnaryFunctorImageFilter< TInputImage, TOutputImage,
                           Functor::ImpulseNoise<
                             typename TInputImage::PixelType,
                             typename TOutputImage::PixelType >   >
{
public:
  /** Standard class typedefs. */
  typedef ImpulseNoiseImageFilter Self;
  typedef UnaryFunctorImageFilter<
    TInputImage, TOutputImage,
    Functor::ImpulseNoise< typename TInputImage::PixelType,
                       typename TOutputImage::PixelType > >  Superclass;

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  typedef typename TOutputImage::PixelType OutputPixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Macro that provides the GetNameOfClass() method */
  itkTypeMacro(ImpulseNoiseImageFilter, UnaryFunctorImageFilter);

  OutputPixelType GetOutputMinimum() const
  {
    return this->GetFunctor().GetOutputMinimum();
  }

  OutputPixelType GetOutputMaximum() const
  {
    return this->GetFunctor().GetOutputMaximum();
  }

  float GetProbability() const { return this->GetFunctor().GetProbability(); }

  void SetOutputMinimum(OutputPixelType min)
  {
    if ( min == this->GetFunctor().GetOutputMinimum() )
      {
      return;
      }
    this->GetFunctor().SetOutputMinimum(min);
    this->Modified();
  }

  void SetOutputMaximum(OutputPixelType max)
  {
    if ( max == this->GetFunctor().GetOutputMaximum() )
      {
      return;
      }
    this->GetFunctor().SetOutputMaximum(max);
    this->Modified();
  }

  void SetProbability(double probability)
  {
    if ( probability == this->GetFunctor().GetProbability() )
      {
      return;
      }
    this->GetFunctor().SetProbability(probability);
    this->Modified();
  }

#ifdef ITK_USE_CONCEPT_CHECKING
  itkConceptMacro( InputConvertibleToOutputCheck,
                   ( Concept::Convertible< typename TInputImage::PixelType, OutputPixelType > ) );
#endif

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf(os, indent);
    os << indent << "Probability: " << static_cast<typename NumericTraits<double>::PrintType>(GetProbability()) << std::endl;
    os << indent << "OutputMinimum: " << static_cast<typename NumericTraits<double>::PrintType>(GetOutputMinimum()) << std::endl;
    os << indent << "OutputMaximum: " << static_cast<typename NumericTraits<double>::PrintType>(GetOutputMaximum()) << std::endl;
    }

protected:
  ImpulseNoiseImageFilter() {}
  virtual ~ImpulseNoiseImageFilter() {}

private:
  ImpulseNoiseImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);     //purposely not implemented
};

}

#endif /* __itkImpulseNoiseImageFilter */
